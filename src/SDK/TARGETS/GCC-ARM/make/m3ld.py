#!/usr/bin/env python3
#-*-coding:utf-8-*-
import configparser
import re
import struct
import os
import sys
import time
import subprocess
import hmac
from enum import Enum, IntEnum, unique
import logging
import shutil,os

class XYUtils:

    def __init__(self):
        pass

    @staticmethod
    def xy_getLogger(logo, loglevel = logging.INFO):
        logger = logging.getLogger(logo)
        logger.setLevel(loglevel)
        console = logging.StreamHandler()
        console.setLevel(loglevel)
        formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
        console.setFormatter(formatter)
        logger.addHandler(console)
        return logger

LOGO = 'xy_m3_packer'
logger = XYUtils.xy_getLogger(LOGO)

class XinyiGlobal(Enum):
    HMAC_KEY = b'XY1100'
    HASH_ALGORITHM = 'SHA1'
    HASH_ARG_NAME_IN_INI = 'xinyihash'

    ARM_MAGIC_NUM = 5847
    ARM_VERSION = 1
    ARM_CONFIG_STRING = '''
    [image0]
    image_core = arm
    image_load_addr = 0x2713F000
    image_exec_addr = 0x2713F000
    image_name = flash.bin
    image_type = bin
    force_exist = True

    [image1]
    image_core = arm
    image_load_addr = 0
    image_exec_addr = 0x2001D000
    image_name = ram.bin
    image_type = bin
    force_exist = True
    '''
    
    DSP_ALLINONE_NAME = 'arm.img'

@unique
class IMAGE_CORE(IntEnum):
    unknown = 0
    arm = 1
    dsp = 2
    nv = 3
    secondary_boot_prime = 4
    secondary_boot_backup = 5
    fast_arm = 6
    fast_dsp = 7
    fast_nv = 8
    fast_secondary_boot_prime = 9
    fast_secondary_boot_backup = 10
    user_nv=20
    rf_nv=21

@unique
class IMAGE_TYPE(IntEnum):
    unknown = 0
    hex_type = 1
    bin_type = 2


#Image信息管理类
class MyImageInfo:
    def __init__(self, image_core_str, image_load_addr_str, image_exec_addr_str, image_name_str, image_type_str, force_exist_str):
        self.image_core = int(IMAGE_CORE[image_core_str])
        self.image_load_addr = int(image_load_addr_str, 16)
        self.image_exec_addr = int(image_exec_addr_str, 16)
        self.image_name = image_name_str
        self.image_type = int(IMAGE_TYPE[image_type_str + '_type'])
        self.force_exist = True if force_exist_str.lower() == 'true' else False
        self._image_bin_content = b''
    
    @property
    def image_bin_content(self):
        return self._image_bin_content
    
    @image_bin_content.setter
    def image_bin_content(self, image_bin_content_arg):
        self._image_bin_content = image_bin_content_arg
    
    def __str__(self):
        return 'image_core:{0}, image_load_addr:{1:x}, image_exec_addr:{2:x}, image_name:{3}, image_type:{4}, force_exist:{5}'.\
            format(self.image_core, self.image_load_addr, self.image_exec_addr, self.image_name, self.image_type, self.force_exist)
    
    def calculate_image_length(self):
       
        pass

#Image中的bin合并类
class MyXYBinsMerger:
    
    def __init__(self, magic_num, version):
        self.magic_num = magic_num
        self.version = version
    
    def get_image_info(self, conf, image_name_bin_dict):
        image_sections = [section for section in conf.sections() if re.match(r'^image\d+$', section)]
        
        image_infos = []
        for image_section in image_sections:
            image_info = MyImageInfo(conf.get(image_section, 'image_core'), conf.get(image_section, 'image_load_addr'), 
                conf.get(image_section, 'image_exec_addr'), conf.get(image_section, 'image_name'), conf.get(image_section, 'image_type'), 
                conf.get(image_section, 'force_exist'))
            image_info.image_bin_content = image_name_bin_dict[conf.get(image_section, 'image_name')]
            image_infos.append(image_info)
        
        return image_infos

    def generate_allinone_file(self, image_infos, allinone_file):
        allinone_bin_content = struct.pack('<L', len(image_infos))
        total_hash = hmac.new(XinyiGlobal.HMAC_KEY.value, digestmod = XinyiGlobal.HASH_ALGORITHM.value)
        with open(allinone_file, 'wb+') as allinone_fp:
            for image_info in image_infos:
                image_hash = hmac.new(XinyiGlobal.HMAC_KEY.value, digestmod = XinyiGlobal.HASH_ALGORITHM.value)
                image_hash.update(image_info.image_bin_content)
                image_sha_digest = image_hash.digest()
                
                allinone_bin_content += struct.pack('<LLLLL', image_info.image_core, image_info.image_load_addr, 
                    image_info.image_exec_addr, image_info.image_type, len(image_info.image_name))
                allinone_bin_content += image_info.image_name.encode()
                allinone_bin_content += struct.pack('<L', len(image_info.image_bin_content))
                allinone_bin_content += image_sha_digest

            for image_info in image_infos:
                allinone_bin_content += image_info.image_bin_content
            
            total_hash.update(allinone_bin_content)
            total_sha_digest = total_hash.digest()
            allinone_fp.write(struct.pack('<LL', self.magic_num, self.version))
            allinone_fp.write(total_sha_digest)
            allinone_fp.write(allinone_bin_content)

    def start_merging(self, configString, image_name_bin_dict, path, allinone_file = 'arm.img'):
        flashSize = os.path.getsize(os.path.join(path,'flash.bin'))

        conf = configparser.ConfigParser()
        conf.read_string(configString)

        flashAddr = int(conf.get('image0', 'image_load_addr'), 16) + flashSize

        if flashAddr % 4096 == 0:
            flashAddr = flashAddr
        else:
            flashAddr = (flashAddr // 4096 + 1) * 4096

        finalAddr = hex(flashAddr)

        conf.set('image1', 'image_load_addr', finalAddr)
        
        image_infos = self.get_image_info(conf, image_name_bin_dict)
        
        self.generate_allinone_file(image_infos, os.path.join(path, allinone_file))


def main():


    if len(sys.argv) < 1:
        logger.error('no enough arguments, exit...')
        sys.exit()

    try:
        image_name_bin_dict = {}
        path = sys.argv[1]

        with open(os.path.join(path,'ram.bin'), 'rb') as f:
            image_name_bin_dict['ram.bin'] = f.read()
 
        with open(os.path.join(path,'flash.bin'), 'rb') as f:
            image_name_bin_dict['flash.bin'] = f.read()

        logger.info('start generating arm allinone')

        MyXYBinsMerger(XinyiGlobal.ARM_MAGIC_NUM.value, XinyiGlobal.ARM_VERSION.value).start_merging(XinyiGlobal.ARM_CONFIG_STRING.value, image_name_bin_dict, path)

        logger.info('arm allinone generated')

    except Exception as e:
        logger.error(e)
        time.sleep(5)

    
if __name__ == '__main__':
    main()