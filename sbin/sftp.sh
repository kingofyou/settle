sftp 10.70.18.121 <<EOF
   user settle settle123
   passive
   cd $ODSPATH
   bin
   get $1
   bye
EOF
