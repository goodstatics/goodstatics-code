import xpc
import serial


seri = serial.Serial("/dev/ttyACM0", baudrate=57600, timeout=1)

with xpc.XPlaneConnect() as client:
    scomi = "sim/cockpit/radios/com1_stdby_freq_hz"
    comi = "sim/cockpit/radios/com1_freq_hz"
    freqlist = []

    def transfer(scom, com):
        tcomi = client.getDREF(com)
        print('tcomi is ', tcomi)
        tscomi = client.getDREF(scom)
        print('tscomi is ', tscomi)
        client.sendDREF(scom, float(tcomi[0]))
        client.sendDREF(com, float(tscomi[0]))

    def xpsend(tune):
        tunels = list(str(tune))
        tunels.remove('.')
        tunestr = ''
        for digit in tunels:
            tunestr = tunestr + digit
        client.sendDREF(scomi, int(tunestr))
        # return int(tunestr)

    while True:
        getdata = seri.readline().decode()
        if getdata != '':
            if getdata == 's\r\n':
                transfer(scomi, comi)
            else:
                xpsend(getdata)


# frequencies: 118-136, do . 10 - 99
