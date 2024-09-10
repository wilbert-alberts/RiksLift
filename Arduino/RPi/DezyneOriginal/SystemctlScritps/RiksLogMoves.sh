while :
do
   mosquitto_sub -W 10 -F "%I %t %p" -t \# 2>/dev/null | grep arrival  >> /home/ben/RiksLog/liftMovements.log
done
