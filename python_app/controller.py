from kivy.uix.widget import Widget
from kivy.uix.slider import Slider
from kivy.properties import NumericProperty, ReferenceListProperty, ObjectProperty
from kivy.vector import Vector

class ZAxisSlider(Slider):
    def on_touch_up(self, touch):
        if touch.grab_current is self:
            self.value = 0
            touch.ungrab(self)
            return True

        return super().on_touch_up(touch)

class MainJoystick(Widget):
    pad_x = NumericProperty(0)
    pad_y = NumericProperty(0)
    pad_pos = ReferenceListProperty(pad_x, pad_y)
    joystick_vector = ObjectProperty(Vector(0, 0))

    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        self.bind(center=self.reset_pad)

    def on_touch_down(self, touch):
        if self.collide_point(*touch.pos):
            touch.grab(self)
            self.update_pad(touch.pos)
            return True
        
        return super().on_touch_down(touch)
    
    def on_touch_move(self, touch):
        if touch.grab_current is self:
            self.update_pad(touch.pos)
            return True

        return super().on_touch_move(touch)
    
    def on_touch_up(self, touch):
        if touch.grab_current is self:
            touch.ungrab(self)
            self.reset_pad()
            return True
        
        return super().on_touch_up(touch)
    
    def update_pad(self, touch_pos):
        target, center = Vector(touch_pos), Vector(self.center)
        distance = Vector(touch_pos).distance(self.center)
        radius = self.width / 2

        if distance > radius:
            direction = (target - center).normalize()
            self.pad_pos = center + direction * radius
            self.joystick_vector = direction
        else:
            self.pad_pos = target
            self.joystick_vector = (target - center) / radius

    def reset_pad(self, *args):
        self.pad_pos = self.center
        self.joystick_vector = Vector(0, 0)