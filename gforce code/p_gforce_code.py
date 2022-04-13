#https://github.com/tinypico/tinypico-micropython/tree/master/lis3dh%20library

import machine, neopixel, time, lis3dh
np = neopixel.NeoPixel(machine.Pin(4),8)
np[0:8] = (50,50,50)
np.write()

i2c = machine.I2C(1, scl = machine.Pin(3), sda = machine.Pin(2))

devices = i2c.scan()

if devices:
    for d in devices:
        print(hex(d))
        
imu = lis3dh.LIS3DH_I2C(i2c)

if imu.device_check():
    imu.range = lis3dh.RANGE_2_G
    print("we found the device")
    
while True:
    x,y,z = [value / lis3dh.STANDARD_GRAVITY for value in imu.acceleration]
    print("x = %0.3f G, y = %0.3f G, z= %0.3f G" % (x,y,z))
    time.sleep(0.1)
    



"""
ctrl_reg1 = 0b10010111 #run at 1.250 Khz with xyz axis enabled no low power
ctrl_reg4 = 0b00000000

i2c.writeto_mem(0x18,0x20,hex(ctrl_reg1)) #slave id , slav reg id, message
i2c.writeto_mem(0x18,0x23,hex(ctrl_reg4))
ready = i2c.readfrom_mem(0x18,0x27,1)
accel_data1 = i2c.readfrom_mem(0x18,0x28,1)
accel_data2 = i2c.readfrom_mem(0x18,0x29,1)

print("ready",ready)
print("xl",accel_data1)
print("xh",accel_data2)
"""

