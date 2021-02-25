from pynq.overlays.base import Baseoverlay
base_overlay = BaseOverlay("base.bit")
while(True):
    for i in range(0,4,1):
        if(base_overlay.buttons[i].read()):
            base_overlay.leds[i].on()
        else:
            base_overlay.leds[i].off()