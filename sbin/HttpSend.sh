#!/bin/sh

#加载环境变量
cd ${HOME}
. .bash_profile

CFGCODE=$1
FILENAME=$2
STLM_DT=$3
KEYPATH="D:\\key\\"
#KEYPATH="/opt/web_server/apache-tomcat-6.0.43_2/key/"

HttpTest ${CFGCODE} ${FILENAME} ${STLM_DT} ${KEYPATH}