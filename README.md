# ArduinoMySQL_PFC

Control de accesos con una placa de desarrollo Arduino, un módulo RFID y con registros almacenados en una base de datos en MySQL. Mediante una placa Arduino MEGA el lector de tarjetas RFID estará preparado para leer en todo momento algún tag. 

Si el ID está registrado y no se supera el aforo se permite el acceso, simbolizado mediante un mensaje por la pantalla LCD y el registro del ID del tag y la hora de acceso se registrarán en una base de datos MySQL. Si por el contrario el ID del tag no es reconocido, no se permitirá el acceso (simbolizado por la pantalla LCD), y el intento de acceso también quedará registrado en la base de datos.

El código en Processing sirve de puente entre la placa Arduino y la base de datos local, ya que no se dispone de conexión a Internet.
