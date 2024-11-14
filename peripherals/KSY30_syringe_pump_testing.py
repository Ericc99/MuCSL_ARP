import time
import serial
import math
import KSY30_pump_config as config

DT_cmd_test_list = {
    "Active": 'QR',
    "Init": 'ZR',
    "In_2.5ml": 'IA24000R',
    "Inlet_to_outlet": 'IA24000M10000A0M1000R',
    "Three_loops_test": 'IA24000M10000A0M1000G3R',
    "Split_drainage": 'IA24000M10000A12000M1000A0R',
}


def DT_cmd_to_byte(input_string):
    # 将每个字符转换为 ASCII 值，并以十六进制格式显示
    b_cmd = "/1" + input_string + "\r"
    byte_array = bytes([ord(char) for char in b_cmd])
    # print(byte_array)
    return byte_array


def init():
    # 配置串口
    ser = serial.Serial('COM6', 9600, timeout=1)
    try:
        if not ser.is_open:
            ser.open()

        # 发送字节序列
        ser.write(DT_cmd_to_byte(DT_cmd_test_list["Active"]))

        # 查看串口是否返回正确启动信息
        resp = ser.readline()
        if str(resp.hex()) == "2f306767030d0a":
            print("Available")
        else:
            print(resp.hex())
            return False
    except serial.SerialException as error:
        print(f"Error: {error}")

    # 阀门校准加预备三联
    ser.write(DT_cmd_to_byte(DT_cmd_test_list["Init"]))
    time.sleep(8)
    ser.write(DT_cmd_to_byte(DT_cmd_test_list["In_2.5ml"]))
    time.sleep(8)
    ser.write(DT_cmd_to_byte(DT_cmd_test_list["Init"]))

    return True


def Volumetric_Converter(vol):
    # 用于将加液计量换算成步进电机步数
    unit_vol = config.FULL_STEPS / config.CHAMBER_VOL
    # 等待1s=1000ms
    wait = 1000
    if vol < 0:
        raise ValueError("Dosage should be a positive number.")
    else:
        # 当输入计量大于腔体最大容积，需要多次往返
        if vol > config.CHAMBER_VOL:
            num_loops = math.floor(vol / config.CHAMBER_VOL)

            # 当输入的容积不为腔体最大容积的整数倍时，最后一次往返按需推进
            if vol % config.CHAMBER_VOL != 0:
                remains = int((vol - num_loops * config.CHAMBER_VOL) * unit_vol)
                if num_loops != 1:
                    loops = "G" + str(num_loops)  # G3 意味着重复三次之前的指令
                else:
                    loops = ""
                cmd = "IA" + str(config.FULL_STEPS) + "M" + str(wait) + "OAO" + loops + "M" + str(wait) + "IA" + str(
                    remains) + "OAO" + "R"  # IA：进液 OAO：出液 M：停几秒 R：收尾命令字符
            else:
                loops = "G" + str(num_loops)
                cmd = "IA" + str(config.FULL_STEPS) + "M" + str(wait) + "OAO" + loops + "M1000" + "R"
        else:
            cmd = "IA" + str(int(vol * unit_vol)) + "M" + str(wait) + "OAO" + "R"
    # print(cmd)
    return cmd


def injection(vol):
    ser = serial.Serial('COM6', 9600, timeout=1)
    try:
        if not ser.is_open:
            ser.open()
        print(DT_cmd_to_byte(Volumetric_Converter(vol)))
        ser.write(DT_cmd_to_byte(Volumetric_Converter(vol)))

    except serial.SerialException as error:
        print(f"Error: {error}")


if __name__ == '__main__':
    if init():
        injection(2.5)  # 加2.5ml
    else:
        print("error")
