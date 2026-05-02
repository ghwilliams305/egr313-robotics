#include <cmath>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_MCP23X17.h>
#include <Adafruit_AS5600.h>
#include <ArduinoJson.h>
#include <BitBang_I2C.h>

//SET PINS FOR I2C
#define I2C_ALPHA_SDA 21
#define I2C_ALPHA_SCL 22
#define I2C_BETA_SDA 18
#define I2C_BETA_SCL 19
#define I2C_GAMMA_SDA 32
#define I2C_GAMMA_SCL 33

//Set pins for soleiod drivers
#define ALPHA_IN 0
#define ALPHA_OUT 1
#define BETA_IN 2
#define BETA_OUT 3
#define GAMMA_IN 4
#define GAMMA_OUT 5
#define BETA_LOCK 6
#define GAMMA_LOCK 7

//Other pins
#define SUCTION 14

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
Adafruit_MCP23X17 mcp;
Adafruit_AS5600 as5600Alpha;
Adafruit_AS5600 as5600Beta;
BBI2C sw_gamma_bus;

TwoWire I2CALPHA = TwoWire(0);
TwoWire I2CBETA = TwoWire(1);

//Defining Arm Values
int lengthAB = 19, lengthBC = 18;
int minAlpha = 0, minBeta = 35, minGamma = 32;
int maxAlpha = 270, maxBeta = 100, maxGamma = 127;
float targetX = 0, targetY = 0, targetZ = 0;
int suction = 0;
float dx = 0, dy = 0, dz = 0;

//Clamping Function
double clamp(double val, double minVal, double maxVal) {
    return fmax(minVal, fmin(maxVal, val));
}

int clampAngle(int angleValue, char angleName) {
    if (angleName == 'a') {
        if (angleValue > maxAlpha) {
            return maxAlpha;
        } else if (angleValue < minAlpha) {
            return minAlpha;
        } else {
            return angleValue;
        }
    } else if (angleName == 'b') {
        if (angleValue > maxBeta) {
            return maxBeta;
        } else if (angleValue < minBeta) {
            return minBeta;
        } else {
            return angleValue;
        }
    } else if (angleName == 'g') {
        if (angleValue > maxGamma) {
            return maxGamma;
        } else if (angleValue < minGamma) {
            return minGamma;
        } else {
            return angleValue;
        }
    }
}

//Proccess D2 Functions generating angles from postion (copied from simulations.py)
//Generating Alpha angle
int generateAlphaAngle(float x = 0, float y = 0) {
    try {

        double alphaRadians = atan2(y, x);
        double alpha = alphaRadians * (180.0 / M_PI);

        if(alpha < minAlpha || alpha > maxAlpha) {
            throw 444;
        }

        return alpha;
    }
    catch (int errorCode) {
        return errorCode;
    }
}

//Generating Beta Angle
int generateBetaAngle(float x = 0, float y = 0, float z = 0) {
    try {
        double r = sqrt(x * x + y * y);
        double rho = sqrt(x * x + y * y + z * z);

        double cosPhi = (pow(lengthAB, 2) + pow(rho, 2) - pow(lengthBC, 2)) / (2 * lengthAB * rho);
        cosPhi = clamp(cosPhi, -1, 1);
        double phi = acos(cosPhi);
        double omega = atan2(z, r);

        double betaRadians = phi + omega;

        double beta = betaRadians * (180.0 / M_PI);

        if(beta < minBeta || beta > maxBeta) {
            throw 444;
        }

        return beta;
    }
    catch (int errorCode) {
        return errorCode;
    }
}

//Generate Gamma angle
int generateGammaAngle(float x = 0, float y = 0, float z = 0) {
    try {
        double rho = sqrt(x * x + y * y + z * z);

        double cosGamma = (pow(lengthAB, 2) + pow(lengthBC, 2) - pow(rho, 2)) / (2 * lengthAB * lengthBC);
        cosGamma = clamp(cosGamma, -1, 1);
        double gammaRadians = acos(cosGamma);

        double gamma = gammaRadians * (180.0 / M_PI);

        if(gamma < minGamma || gamma > maxGamma) {
            throw 444;
        }

        return gamma;
    }
    catch (int errorCode) {
        return errorCode;
    }
}

//Reversing D2 Functions
//Generating Pos based on input
int generatePos(int alpha, int beta, int gamma, char pos = 'z') {
    double alphaRadian = alpha * (M_PI / 180.0);
    double betaRadian = beta * (M_PI / 180.0);
    double gammaRadian = gamma * (M_PI / 180.0);

    if (pos == 'x') {
        return (lengthAB * cos(betaRadian) - lengthBC * cos(gammaRadian + betaRadian)) * cos(alphaRadian);
    } else if (pos == 'y') {
        return (lengthAB * cos(betaRadian) - lengthBC * cos(gammaRadian + betaRadian)) * sin(alphaRadian);
    } else {
        return lengthAB * sin(betaRadian) - lengthBC * sin(gammaRadian + betaRadian);
    }
}

void setup() {
    //Setup Function
    //Get Starting
    Serial.begin(115200);
    I2CALPHA.begin(I2C_ALPHA_SDA, I2C_ALPHA_SCL, 100000);
    I2CBETA.begin(I2C_BETA_SDA, I2C_BETA_SCL, 100000);

    SerialBT.begin("roboticARM");
    Serial.println("The device started, now you can pair it with bluetooth!");

    //ADD PORTS
    if (!mcp.begin_I2C(0x20, &I2CALPHA)) {
        Serial.println("MCP Not Found");
        while(1);
    }

    Serial.println("MCP found!");

    //if (!as5600Alpha.begin(0x36, &I2CALPHA)) {
    //    Serial.println("Could not find as5600 Alpha, checking!");
    //    while (1)
    //        delay(10);
    //}

    //Serial.println("as5600 Alpha found!");

    //if (!as5600Beta.begin(0x36, &I2CBETA)) {
    //    Serial.println("Could not find as5600 Beta, checking!");
    //    while (1)
    //        delay(10);
    //}

    //Serial.println("as5600 Beta found!");

    //Set Up Virtual Gamma
    //memset(&sw_gamma_bus, 0, sizeof(sw_gamma_bus));
    //sw_gamma_bus.bWire = 0;
    //sw_gamma_bus.iSDA = I2C_GAMMA_SDA;
    //sw_gamma_bus.iSCL = I2C_GAMMA_SCL;
    //I2CInit(&sw_gamma_bus, 100000);

    //Serial.println("as5600 Gamma Initialized");

    //Configuring the AS5600 sensors (Alpha, Beta, and Gmma)
    //as5600Alpha.enableWatchdog(false);
    //as5600Alpha.setPowerMode(AS5600_POWER_MODE_NOM);
    //as5600Alpha.setHysteresis(AS5600_HYSTERESIS_OFF);
    //as5600Beta.enableWatchdog(false);
    //as5600Beta.setPowerMode(AS5600_POWER_MODE_NOM);
    //as5600Beta.setHysteresis(AS5600_HYSTERESIS_OFF);

    //as5600Alpha.setOutputStage(AS5600_OUTPUT_STAGE_ANALOG_FULL);
    //as5600Beta.setOutputStage(AS5600_OUTPUT_STAGE_ANALOG_FULL);

    //as5600Alpha.setSlowFilter(AS5600_SLOW_FILTER_16X);
    //as5600Alpha.setFastFilterThresh(AS5600_FAST_FILTER_THRESH_SLOW_ONLY);
    //as5600Beta.setSlowFilter(AS5600_SLOW_FILTER_16X);
    //as5600Beta.setFastFilterThresh(AS5600_FAST_FILTER_THRESH_SLOW_ONLY);

    //as5600Alpha.setZPosition(0);
    //as5600Alpha.setMPosition(4095);
    //as5600Alpha.setMaxAngle(4095);
    //as5600Beta.setZPosition(0);
    //as5600Beta.setMPosition(4095);
    //as5600Beta.setMaxAngle(4095);

    //Set Up Suction Value
    pinMode(SUCTION, OUTPUT);

    //configure soleoid pins
    mcp.pinMode(ALPHA_IN, OUTPUT);
    mcp.pinMode(ALPHA_OUT, OUTPUT);
    mcp.pinMode(BETA_IN, OUTPUT);
    mcp.pinMode(BETA_OUT, OUTPUT);
    mcp.pinMode(GAMMA_IN, OUTPUT);
    mcp.pinMode(GAMMA_OUT, OUTPUT);
    mcp.pinMode(BETA_LOCK, OUTPUT);
    mcp.pinMode(GAMMA_LOCK, OUTPUT);
}

void loop() {
    //Finding Where the robot is
    //Get Sensor Values
    uint16_t alphaAngle = 0;//as5600Alpha.getAngle();
    int alphaAngleDegrees = (alphaAngle * 360) / 4095;
    uint16_t betaAngle = 0;//as5600Beta.getAngle();
    int betaAngleDegrees = (betaAngle * 360) / 4095;
    int gammaAngleDegrees = 0;

    //Get Gamma Sensor Value
    //uint8_t reg = 0x0E;
    //I2CWrite(&sw_gamma_bus, 0x36, &reg, 1);

    //uint8_t buffer[2];
    //if (I2CReadRegister(&sw_gamma_bus, 0x36, 0x0E, buffer, 2)) {
    //    uint16_t gammaAngle = ((uint16_t)buffer[0] << 8) | buffer[1];
    //    gammaAngleDegrees = (gammaAngle * 360) / 4095;
    //} else {
    //    Serial.println("Gamma data not found");
    //}

    alphaAngleDegrees = clampAngle(alphaAngleDegrees, 'a');
    betaAngleDegrees = clampAngle(betaAngleDegrees, 'b');
    gammaAngleDegrees = clampAngle(gammaAngleDegrees, 'g');

    //Get Current Position
    float currentX = generatePos(alphaAngleDegrees, betaAngleDegrees, gammaAngleDegrees, 'x');
    float currentY = generatePos(alphaAngleDegrees, betaAngleDegrees, gammaAngleDegrees, 'y');
    float currentZ = generatePos(alphaAngleDegrees, betaAngleDegrees, gammaAngleDegrees, 'z');

    //Finding where the robot is going
    //Set Up and get position change
    if (SerialBT.available()) {
        String received = SerialBT.readStringUntil(']') + ']';
        received.trim();

        if (received.length() > 0) {
            JsonDocument doc;
            DeserializationError error = deserializeJson(doc, received);

            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
            } else {
                int sensitivity = 100;
                dx = doc[0];
                dy = doc[1];
                dz = doc[2];

                suction = doc[3];
            }
        }
    }

    //Changing position
    targetX = currentX + dx;
    targetY = currentY + dy;
    targetZ = currentZ + dz;

    //Getting Target Angles
    int targetAlpha = generateAlphaAngle(targetX, targetY);
    if (targetAlpha == 444) {
        targetAlpha = alphaAngleDegrees;
    }
    int targetBeta = generateBetaAngle(targetX, targetY, targetZ);
    if (targetBeta == 444) {
        targetBeta = betaAngleDegrees;
    }
    int targetGamma = generateGammaAngle(targetX, targetY, targetZ);
    if (targetGamma == 444) {
        targetGamma = gammaAngleDegrees;
    }

    Serial.print("Current: (");
    Serial.print(alphaAngleDegrees);
    Serial.print(", ");
    Serial.print(betaAngleDegrees);
    Serial.print(", ");
    Serial.print(gammaAngleDegrees);
    Serial.print(") -> ");
    Serial.print("(");
    Serial.print(dx);
    Serial.print(", ");
    Serial.print(dy);
    Serial.print(", ");
    Serial.print(dz);
    Serial.print(", ");
    Serial.print(suction);
    Serial.print(") -> ");
    Serial.print("Target: (");
    Serial.print(targetAlpha);
    Serial.print(", ");
    Serial.print(targetBeta);
    Serial.print(", ");
    Serial.print(targetGamma);
    Serial.print(")");

    //Seting Piston Trajectories
    if (std::round(dx) > 0) {
        mcp.digitalWrite(ALPHA_OUT, HIGH);
        mcp.digitalWrite(ALPHA_IN, LOW);
    } else if (std::round(dx) < 0) {
        mcp.digitalWrite(ALPHA_OUT, LOW);
        mcp.digitalWrite(ALPHA_IN, HIGH);
    } else {
        mcp.digitalWrite(ALPHA_OUT, LOW);
        mcp.digitalWrite(ALPHA_IN, LOW);
    }

    if (std::round(dy) < 0) {
        mcp.digitalWrite(BETA_LOCK, HIGH);
        mcp.digitalWrite(BETA_OUT, HIGH);
        mcp.digitalWrite(BETA_IN, LOW);
    } else if (std::round(dy) > 0) {
        mcp.digitalWrite(BETA_LOCK, HIGH);
        mcp.digitalWrite(BETA_OUT, LOW);
        mcp.digitalWrite(BETA_IN, HIGH);
    } else {
        mcp.digitalWrite(BETA_LOCK, LOW);
        mcp.digitalWrite(BETA_OUT, LOW);
        mcp.digitalWrite(BETA_IN, LOW);
    }

    if (std::round(dz) > 0) {
        mcp.digitalWrite(GAMMA_LOCK, HIGH);
        mcp.digitalWrite(GAMMA_OUT, HIGH);
        mcp.digitalWrite(GAMMA_IN, LOW);
    } else if (std::round(dz) < 0) {
        mcp.digitalWrite(GAMMA_LOCK, HIGH);
        mcp.digitalWrite(GAMMA_OUT, LOW);
        mcp.digitalWrite(GAMMA_IN, HIGH);
    } else {
        mcp.digitalWrite(GAMMA_LOCK, LOW);
        mcp.digitalWrite(GAMMA_OUT, LOW);
        mcp.digitalWrite(GAMMA_IN, LOW);
    }

    if (suction == 1) {
        digitalWrite(SUCTION, HIGH);
    } else {
        digitalWrite(SUCTION, LOW);
    }

    Serial.println();
    delay(50);
}