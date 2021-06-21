#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import serial
from time import sleep
import argparse
import datetime
import time
import configparser

conf = configparser.ConfigParser()
conf.read('config.ini')
ComPort = conf.get('info', 'ComPort')

RETURN_CODE_ACK = 0x61
RETURN_CODE_AUTOBAUD_DONE = 0x6B
RETURN_CODE_AUTOBAUD_AGAIN = 0x6C
RETURN_CODE_INVALID_CHECKSUM = 0x67

BYTE_WRITE_MEMORY_CMD = 0x12
BYTE_READ_MEMORY_CMD = 0x13

LOGO = 'Flash Dump'

class MyXYFileUploader:

    def __init__(self, serialPort = 'COM5', bautRate = 921600, dataBits = 8, stopBits = 1, parityBits = None, timeout = 5.0):
        self.seri = serial.Serial(serialPort, bautRate, timeout = timeout)
        if self.seri.isOpen() :
            print("open success")
        else :
            print("open failed")
            raise ValueError

    def auto_bautrate(self):
        self.seri.write(b'\xC0')
        data = self.seri.read(1)
        if data is None or len(data) < 1:
            print('Error! Uart Receive Timeout!')
            return False
        #print('{0:x}'.format(data[0]))
        if data[0] == RETURN_CODE_AUTOBAUD_AGAIN:
            print('Warning! Uart Auto-Baudrate Already Done!')
            return True
        elif data[0] != RETURN_CODE_AUTOBAUD_DONE:
            print('Error! Uart Auto-Baudrate Set Fail!')
            return False
        print('Uart Auto-Baudrate Set OK!')
        return True

    def uploading_binary(self, addr, data_len, filename):
        with open(filename, 'wb+') as f:
            uart_tx_bytes = bytearray(9)
            uart_tx_bytes[0] = BYTE_READ_MEMORY_CMD
            uart_tx_bytes[1] = 0xFF - BYTE_READ_MEMORY_CMD
            self.seri.write(uart_tx_bytes[0:2])
            data = self.seri.read(1)
            print('{0:02x}'.format(data[0]))
            if data is None or len(data) < 1:
                print('Error! Uart Receive Timeout!')
                return False
            if data[0] != RETURN_CODE_ACK:
                print('Error! Uart Receive Not ACK!')
                return False

            data_len_byte = data_len if data_len % 4 == 0 else (data_len // 4 + 1) * 4
            data_len_word = data_len_byte >> 2;

            uart_tx_bytes[0] = (addr) & 0xFF
            uart_tx_bytes[1] = (addr >> 8) & 0xFF
            uart_tx_bytes[2] = (addr >> 16) & 0xFF
            uart_tx_bytes[3] = (addr >> 24) & 0xFF

            uart_tx_bytes[4] = (data_len_byte) & 0xFF
            uart_tx_bytes[5] = (data_len_byte >> 8) & 0xFF
            uart_tx_bytes[6] = (data_len_byte >> 16) & 0xFF
            uart_tx_bytes[7] = (data_len_byte >> 24) & 0xFF

            uart_tx_bytes[8] = 0
            for i in range(8):
                uart_tx_bytes[8] ^= uart_tx_bytes[i]
            self.seri.write(uart_tx_bytes[0:9])
            data = self.seri.read(1)
            if data is None or len(data) < 1:
                print('Error! Uart Receive Timeout!')
                return False
            if data[0] != RETURN_CODE_ACK:
                print('Error! Uart Receive Not ACK!')
                return False

            msg_len_byte = 256
            msg_len_word = msg_len_byte // 4
            msg_num = data_len_word // msg_len_word
            msg_left_word = data_len_word % msg_len_word

            read_len_byte = 0

            for i in range(msg_num):
                print('start reading bytes from {0:x} to {1:x}'.format(addr + read_len_byte, addr + read_len_byte + 256))
                data = self.seri.read(261)
                if data is None or len(data) < 261:
                    print('Error! Uart Receive Timeout!')
                    print(data.hex())
                    print(len(data))
                    return False

                checksum = 0
                for j in range(msg_len_byte + 4):
                    checksum ^= data[j]
                if checksum != data[j + 1]:
                    uart_tx_bytes[0] = RETURN_CODE_INVALID_CHECKSUM
                    self.seri.write(uart_tx_bytes[0:1])
                    return False
                uart_tx_bytes[0] = RETURN_CODE_ACK
                self.seri.write(uart_tx_bytes[0:1])
                #print(data[4 : len(data) - 1].hex())
                f.write(data[4 : len(data) - 1])
                read_len_byte += msg_len_byte

            if msg_left_word != 0:
                print('start reading bytes from {0:x} to {1:x}'.format(addr + read_len_byte, addr + read_len_byte + msg_left_word * 4))
                data = self.seri.read(msg_left_word * 4 + 5)
                if data is None or len(data) < msg_left_word * 4 + 5:
                    print('Error! Uart Receive Timeout!')
                    return False
                checksum = 0
                for j in range(msg_left_word * 4 + 4):
                    checksum ^= data[j]
                if checksum != data[j + 1]:
                    print(data.hex())
                    print(checksum)
                    uart_tx_bytes[0] = RETURN_CODE_INVALID_CHECKSUM
                    self.seri.write(uart_tx_bytes[0:1])
                    return False
                uart_tx_bytes[0] = RETURN_CODE_ACK
                self.seri.write(uart_tx_bytes[0:1])
                f.write(data[4 : len(data) - 1])
                read_len_byte += msg_left_word * 4
        print('upload finished')
        return True

    def start_uploading(self, addr, data_len, filename):
        if not self.auto_bautrate():
            return False
        print(datetime.datetime.now())
        self.uploading_binary(addr, data_len, filename)
        print(datetime.datetime.now())
        self.seri.close()

def main():
    # parser = argparse.ArgumentParser(description = "%s is for reading flash data of Xinyi NBIoT" % LOGO, prog = LOGO)
    # parser.add_argument("-c", "--ComPort", type = str, default = 'COM4', help = 'default ComPort is "COM4"')
    # args = parser.parse_args()
    
    BautRate=921600
    StartAddr=0x271BC000
    Length=0x44000
    OutputFile='0x20000000_44000.bin'
    myXYFileUploader = MyXYFileUploader(ComPort,  BautRate, timeout=3)
    myXYFileUploader.start_uploading( StartAddr,  Length,  OutputFile)
    
    time.sleep(4)
    
    BautRate=921600
    StartAddr=0x271BC000
    Length=0x2A000
    OutputFile='0x3ffc0000_29800.bin'
    myXYFileUploader = MyXYFileUploader(ComPort,  BautRate, timeout=3)
    myXYFileUploader.start_uploading( StartAddr,  Length,  OutputFile)
    time.sleep(4)
    
    BautRate=921600
    StartAddr=0x271E6000
    Length=0x12000
    OutputFile='0x20047000_12000.bin'
    myXYFileUploader = MyXYFileUploader(ComPort,  BautRate, timeout=3)
    myXYFileUploader.start_uploading( StartAddr,  Length,  OutputFile)
    time.sleep(4)
    
    BautRate=921600
    StartAddr=0x271FB000
    Length=0x3000
    OutputFile='dsp_current_heap.bin'
    myXYFileUploader = MyXYFileUploader(ComPort,  BautRate, timeout=3)
    myXYFileUploader.start_uploading( StartAddr,  Length,  OutputFile)
    
    time.sleep(4)
    BautRate=921600
    StartAddr=0x271F8000
    Length=0x3000
    OutputFile='0x2005d000_3000.bin'
    myXYFileUploader = MyXYFileUploader(ComPort,  BautRate, timeout=3)
    myXYFileUploader.start_uploading( StartAddr,  Length,  OutputFile)
    
    


if __name__ == '__main__':
    main()