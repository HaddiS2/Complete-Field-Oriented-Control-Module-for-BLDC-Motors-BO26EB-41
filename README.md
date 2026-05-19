# Complete-Field-Oriented-Control-Module-for-BLDC-Motors-BO26EB-41
Custom field-oriented control (FOC) motor controller with USB-C Power Delivery.

This repository contains the hardware design files, firmware, and test rig for a custom brushless motor controller built around field-oriented control (FOC). The board negotiates its supply voltage over USB-C Power Delivery using an STUSB4500, and measures rotor position with a TLE5009 magnetic encoder mounted directly beneath the motor shaft. The PCB was designed in KiCad, and a custom test rig — modeled in Fusion 360 and 3D printed — holds the motor and controller in fixed alignment to keep the encoder-to-magnet spacing constant for repeatable position- and velocity-control testing.
