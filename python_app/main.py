import json

from kivy.app import App
from kivy.lang import Builder
from kivy.clock import Clock
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.anchorlayout import AnchorLayout
from kivy.logger import Logger

from controller import MainJoystick, ZAxisSlider
from bluetooth_interface import get_socket_stream

class Controlls(BoxLayout):
    pass

class ControllsBox(AnchorLayout):
    pass

Builder.load_file('ui/main.kv')
Builder.load_file('ui/control-screen.kv')

class RobotControllsApp(App):
    cooridinate_change = (0, 0, 0, 0)

    def build(self):
        root = Controlls()

        joystick_box = ControllsBox(size_hint=(0.5, 1))
        self.joystick = MainJoystick()
        root.add_widget(joystick_box)
        joystick_box.add_widget(self.joystick)

        lift_box = ControllsBox(size_hint=(0.5, 1))
        self.z_dail = ZAxisSlider(
            orientation='vertical',
            min=-1, max=1, value=0,
            size_hint=(None, 0.8), width=50,
            pos_hint={'right':0.9, 'center_y': 0.5}
        )
        root.add_widget(lift_box)
        lift_box.add_widget(self.z_dail)

        self.recv_stream, self.send_stream = get_socket_stream('roboticARM')

        Clock.schedule_interval(self.main_loop, 1/60)

        return root
    
    def main_loop(self, dt):
        sensitivity = 100
        x = round(self.joystick.joystick_vector.x * sensitivity) / sensitivity
        y = round(self.joystick.joystick_vector.y * sensitivity) / sensitivity
        z = round(self.z_dail.value * sensitivity) / sensitivity
        w = self.joystick.suction

        cooridinate_change = (x, y, z, w)
        if cooridinate_change == self.cooridinate_change:
            return 0
        
        self.cooridinate_change = cooridinate_change
        data_to_send = json.dumps(cooridinate_change)

        if self.send_stream is not None:
            self.send_stream.write(data_to_send.encode('utf-8'))
            self.send_stream.flush()
            print(f'Cooridinate => (dx, dy, dz) - > {data_to_send}')

if __name__ == '__main__':
    RobotControllsApp().run()