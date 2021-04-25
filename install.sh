cp ./sysprep.service /etc/systemd/system/
systemctl daemon-reload
systemctl enable sysprep.service
