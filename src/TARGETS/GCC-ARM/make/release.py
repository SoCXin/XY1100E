#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys


class ReleaseVersion:
    def __init__(self, top_dir):
        # 根目录，外发时的最外层目录
        self.top_dir = os.path.abspath(top_dir)
        # make文件夹所在的目录
        self.gcc_dir = os.path.join(self.top_dir, 'TARGETS', 'GCC-ARM')
        # 要修改的文件
        self.modify_file = os.path.join(self.gcc_dir, 'make', 'reduce.mk')
        # 需要删除的共仓文件夹
        self.common_list = ['App_common', 'Basic_common']

    @staticmethod
    def remove_folder_file(path):
        # 获取目标路径的文件列表
        file_list = list()
        for root, dirs, files in os.walk(path):
            for name in files:
                path_name = os.path.join(root, name)
                file_list.append(path_name)
        # 循环删除每个文件
        for cur_file in file_list:
            os.remove(cur_file)

    @staticmethod
    def remove_empty_folder(path):
        # 递归删除所有空目录，topdown=False 的作用是从里层文件夹开始遍历，实现递归删除
        for root, dirs, files in os.walk(path, topdown=False):
            # 这里这样判断才能完全递归删除所有空目录
            if not os.listdir(root):
                os.rmdir(root)

    def remove_folder(self, foldr_path):
        # 删除路径内的所有文件
        self.remove_folder_file(foldr_path)
        # 删除所有空文件夹
        self.remove_empty_folder(foldr_path)

    def delete_common_file(self):
        # 循环删除每一个共仓文件夹
        for cur_common in self.common_list:
            # 获取路径，作为入参，删除整个文件夹所有内容
            folder_path = os.path.join(self.top_dir, cur_common)
            if os.path.exists(folder_path):
                self.remove_folder(folder_path)

    def delete_special_file(self):
        # 不同目录中需要删除的文件列表
        delete_top_dir_file_list = list()
        delete_trace_dir_file_list = list()
        delete_make_dir_file_list = list()
        # 根目录中需要删除的文件
        delete_top_dir_file_list.append('post-checkout')
        delete_top_dir_file_list.append('git免密码.bat')
        delete_top_dir_file_list.append('共仓文件拷贝.bat')
        delete_top_dir_file_list.append('共仓文件拷贝免对比版.bat')
        # trace脚本目录中需要删除的文件
        delete_trace_dir_file_list.append('FreeRTOS_Heap6.cmm')
        delete_trace_dir_file_list.append('FreeRTOS_Heap7.cmm')
        delete_trace_dir_file_list.append('FreeRTOS_MainStack.cmm')
        delete_trace_dir_file_list.append('FreeRTOS_Task.cmm')
        delete_trace_dir_file_list.append('FreeRTOS_Timer.cmm')
        delete_trace_dir_file_list.append('HardFault.cmm')
        # make目录中需要删除的文件
        delete_make_dir_file_list.append('release.py')
        # 需要删除的文件路径列表
        delete_file_path_list = list()
        # 根目录中需要删除的文件的路径保存到列表中
        for cur_file in delete_top_dir_file_list:
            # 获取文件的路径
            file_path = os.path.join(self.top_dir, cur_file)
            # 如果文件存在，则加入待删除列表
            if os.path.exists(file_path):
                delete_file_path_list.append(file_path)
        # trace脚本目录中需要删除的文件的路径保存到列表中
        for cur_file in delete_trace_dir_file_list:
            # 获取文件的路径
            file_path = os.path.join(self.gcc_dir, 'trace_scripts', 'M3_FreeRTOS', cur_file)
            # 如果文件存在，则加入待删除列表
            if os.path.exists(file_path):
                delete_file_path_list.append(file_path)
        # make目录中需要删除的文件的路径保存到列表中
        for cur_file in delete_make_dir_file_list:
            # 获取文件的路径
            file_path = os.path.join(self.gcc_dir, 'make', cur_file)
            # 如果文件存在，则加入待删除列表
            if os.path.exists(file_path):
                delete_file_path_list.append(file_path)
        # 删除指定文件
        for delete_file in delete_file_path_list:
            os.remove(delete_file)

    def move_arm_bin_to_top_dir(self):
        # 文件名为 arm.img
        file_name = 'arm.img'
        # 旧文件路径
        old_file_path = os.path.join(self.gcc_dir, 'bin', file_name)
        # 新文件路径
        new_file_path = os.path.join(self.top_dir, file_name)
        # 如果文件存在，则移动到指定目录
        if os.path.exists(old_file_path):
            os.rename(old_file_path, new_file_path)

    def modify_reduce_file(self):
        # 文件不存在是，直接退出
        if not os.path.exists(self.modify_file):
            return
        # 读取文件内所有内容
        with open(self.modify_file, 'r') as f_in:
            file_context = f_in.readlines()
            f_in.close()
        # 遍历每行的内容，如满足修改条件，则进行修改
        for index in range(0, len(file_context)):
            # 获取当前行内容
            cur_line = file_context[index]
            # 以 '#' 开头，则该行是注释行，无需修改
            if cur_line.startswith('#'):
                continue
            # 以 '#' 为目标划分字符串，如果包含该子串，则返回长度为3的元组 (pre_sub, sub, fol_sub)
            # 否则返回长度为2的元组 ('原字符串', ', ')
            tuple_line = cur_line.partition('#')
            # 如果第二个成员为 '#'，则说明该行包含 '#'，后面存在注释，取的注释前的内容
            tar_context = tuple_line[0]
            # 判断字符串中是否只有一个 '='，这种情况才认为这是真正需要修改的行
            if tar_context.count('=') == 1:
                tuple_valid = tar_context.partition('=')
                # 这里可以不用判断，上面的条件已经保证了这个条件是满足的
                if len(tuple_valid) == 3 and tuple_valid[1] == '=':
                    # 替换 'b' 到 'c'
                    new_context = tuple_valid[2].replace('b', 'c')
                    # 新的有效行
                    new_line = tuple_valid[0] + tuple_valid[1] + new_context
                    # 当该行存在注释时，需要把注释的内容追加在后面，并替换原先行的内容
                    if len(tuple_line) == 3:
                        new_line = new_line + tuple_line[1] + tuple_line[2]
                    # 新内容覆盖到原先的内容
                    file_context[index] = new_line
        # 以覆盖写入的方式，覆盖原先文件的内容
        with open(self.modify_file, 'w') as f_out:
            write_str = str().join(file_context)
            f_out.write(write_str)
            f_out.close()

    def release_doing(self):
        # 获取当前目录
        cur_dir = os.getcwd()
        # 如果顶层目录不是当前目录，则需要切换到顶层目录
        if cur_dir != self.top_dir:
            os.chdir(self.top_dir)
        # 拷贝共仓文件
        if os.path.exists('共仓文件拷贝.bat'):
            os.system('共仓文件拷贝.bat')
        # 文件拷贝完成后，即可删除共仓文件
        self.delete_common_file()
        # 删除某些特殊文件，这些文件不应该外发
        self.delete_special_file()
        # 开始第一次编译，并生成库
        os.system('make all')
        # 修改封库的makefile文件
        self.modify_reduce_file()
        # 删除封库的源码
        os.system('make clean')
        # 再编译一次，确保好库外发能够正常编译
        os.system('make all')
        # 编译成功后，在clean之前，把可执行文件移动到根目录
        self.move_arm_bin_to_top_dir()
        # 编译成功，测试完成，再次clean，删除中间文件
        os.system('make clean')
        # 如果顶层目录不是当前目录，则切换回之前的目录
        if cur_dir != self.top_dir:
            os.chdir(cur_dir)


def main():
    # 当传入参数长度小于等于1时（不会小于1），即没有传参，则认为是当前目录网上4级父目录是根目录
    if len(sys.argv) <= 1:
        cur_dir = os.getcwd()
        top_dir = os.path.join(cur_dir, os.pardir, os.pardir, os.pardir, os.pardir)
    # 否则认为传入的参数就是根目录路径
    else:
        top_dir = sys.argv[1]
    # 测试用
    # top_dir = r'G:\pycharm-workspace\copy\xinyiNBIot_M3_v2'
    # 生成对象
    release = ReleaseVersion(top_dir)
    # 执行操作，完成外发
    release.release_doing()
    # 打印外发成功
    print("\n>>>Release Success!!!")
    # 输入内容，只是为暂停控制台，用来查看结果
    input("\n>>>Stop to see result")


if __name__ == '__main__':
    main()
