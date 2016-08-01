#!/bin/sh

file_name=openwrt-x86-generic-combined-ext4
input=$file_name.img
format=vmdk
output=$file_name.$format

uuid=$1
uuid:=`cat openwrt.vbox | grep output | grep -o "{.*}" | sed '1s/[{}]//g'`

cp /home/tristan_xiao/project/openwrt/bin/x86/$input.gz ./
gunzip -f $input.gz
[ -f $output ] && rm $output
vboxmanage convertfromraw --format VMDK $input $output

VBoxManage internalcommands sethduuid $output $uuid

