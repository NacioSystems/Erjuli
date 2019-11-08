# Erjuli

## de NacioSystems:

Versión 3.0
Fecha: 5/11/2019

Este proyecto es robot de minisumo llamado **Erjuli**, pensado para las competiciones de un máximo de 500gr y dimensiones de 10x10cm2. Realizado por NacioSystems (O Milladoiro):

Es la tercera versión de este robot, la primera participó en la Oshwdem 2016, tenía una caja de madera y una balda desplegable, En la segunda versión se cambió la caja de madera por una impresa en 3D con PLA y se le añadió un micorservo en la parte trasera, para activar un brazo que bajaba la muleta que intentaba torear a los otros robots competidores. En esta tercera versión se eliminó la balda desplegable, por otra fija de material flexible pensada para el mismo efecto y se retocó el software para adaptarlo a las tres fases de la competición


- - -

### Introducción:

El chasis del vehículo está construido sobre la plataforma **Pololu Zumo Arduino**, sobre el que se instalaron sensores de distancia a través de ultrasonidos **HC-SR04**, uno en la dirección frontal y otro a cada lado del bastidor. Se complementa con el sensor **QTR** frontal de Pololu Zumo, que tendrá la función de detectar la línea del borde de la pista, con el objeto de no salirse.

El vehículo se enciende con el interruptor de la _shield_ de Pololu_, y queda parado a la espera de que se pulse el botón incorporado en el chasis, para iniciar el combate. Una vez pulsado el botón **Erjuli** cuenta los 5 segundos de rigor que marcan las reglas. Una vez en marcha lee a través de los sensores en que situación está el contrincante, delante, a la derecha o a la izquierda. Lógicamente si no está en ninguna de esas posiciones es que estará detrás. **Erjuli** girará en la dirección en la que encuentre al contrincante y acelerará para embestirlo con sus 500gr de peso. Si en el movimiento detecta que pisa el borde de la pista, con los sensores delanteros QTR, retrocederá para salir de la zona  de peligro.


![Erjuli](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Erjuli%20final%201.jpg "Erjuli listo")

Los sensores **HC-SR04** se activan simultáneamente a la vez, leyendo cada uno de ellos su propia señal y la de los otros sensores de modo sincronizado, hasta un rango de 50cm. Si detecta un objeto o contrincante en alguno de los lados, girará hasta detectarlo con el sensor frontal, pero este giro será por un tiempo limitado,para evitar quedarse en la misma posición expuesto al ataque del adversario.

![Sensores 3D](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Interior%20Carcasa.JPG "Situación sensores")

Al mismo tiempo **Erjuli** moverá su servomotor trasero, para poner la muleta del lado en que encuentra el adversario, para que este se confunda con el señuelo


### Materiales:

El vehículo utiliza los siguientes materiales:
* Pololu Zumo for Arduino con balda
* Arduino UNO R3
* 3 x HC-SR04
* Zumo Reflectante Sensor Array
* Microsergo SG90
* Piezas impresas en 3D PLA
* CI perforado universal para la conexión de los sensores de ultrasonidos
* Tiras de pines hembra y cableado para la conexión de todos los sistemas a la _shield_

![Foto Erjuli desarmado](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Desarmado.JPG "Erjuli desarmado")

Otros materiales:

Para ocultar toda la electrónica y cableado, así como para darle una mayor rigidez al conjunto, incluyendo la fijación del micorservo y su brazo para la muleta, se ha creado una caja impresa en 3D, que incluye los orificios para los sensores de ultrasonidos y el microservo SG90.

![Foto Erjuli 3D](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Erjuli%201.JPG "Erjuli 3D")


### Dohyo:

El **_Dohyo_** es pista donde se desenvuelve el combate de minisumo. Es una circunferencia de 77cm de diámetro de color negro. El borde exterior del _Dohyo_ es una línea blanca de 2,5cm de ancho y que sirve para indicarles a los robóts los límites que no se deben traspasar.

![Foto Dohyo](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Dojo.png "Medidas Dohyo")

Durante cada combate cada robot intentará expulsar al otro del Dohyo, ganando el que quede dentro o salga en último lugar.

### Condiciones iniciales:

La competición se desenvuelve en hasta tres asaltos. En el primer asalto, los robóts estarán enfrentados sobre unas marcas equiespaciadas del centro del _Dohyo_. En el segundo asalto los contrincantes estarán en la misma posición de inicio pero de tal manera que el lado derecho de uno será paralelo al lado izquierdo del otro, mirando en direcciones opuestas. En el tercer asalto, si no quedó claro cual es el vencedor en los dos primeros asaltos, los robóts se pondrán de espaldas, mirando hacia el exterior del _Dohyo_.

Cada asalto comienza cuando lo indica el árbitro, dejando un espacio temporal de 5 segundos antes de empezar a moverse.


### Detección del contrincante y estrategia:
Erjuli, que torea por chicuelinas, localiza la posición del contrincante leyendo los sensores, de tal manera que si está en frente acelerá para embestirlo, mientras le pasa unos muletazos a la embestida del rival. En el caso de que detecte al contrincante por uno de los flancos, moverá la muleta hacie ese flanco y girará al mismo tiempo dando un gran pase al rival y embistiendo una vez que lo tiene de frente. En caso de detectar el borde retrocederá y girará para mantenerse en el Dohyo.

En función del asalto, Erjuli empezará primero embistiendo de frente, para intentar obtener la mayor inercia posible en el choque inicial. En el segundo y tercer asalto, Erjuli girará para volver a embestir al contrincante una vez que lo encuentra de frente.

Erjuli utilizará su balda para levantar al otro contrincante y hacerle perder adherencia, para tener una posición dominante con su peso aumentado por el peso del otro robot levantado.


### Programa:

El programa está realizado con el IDE Arduino, para su programación directa a través de cable USB. En la carpeta software se puede ver el programa comentado. El programa cuenta con una pestaña de "configuración.h", para ajustar los valores por defecto, que dependerán de los motores y sensores utilizados en cada caso, si son diferentes a los de esta construcción.

Una vez encendido el robót configura los sensores, y dependiendo de las pulsaciones del botón de inicio sabrá en que estado estará, asalto primero, segundo o tercero. De esa manera puede empezar a moverse de diferente manera, es decir, en el primer asalto tendrá que avanzar para alcanzar la máxima velocidad lo antes posible para la colisión frontal con el adversario. Lógicamente ganará el que tenga mayor inercia (masa x velocidad) o el que consiga levantar al contrincante. Si lo levanta aumentará su peso y reducirá la adherencia del contrario.

La muleta del _Erjuli_ servirá para despistar al contrincante.

![Muleta Erjuli](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Muleta%20Erjuli.JPG "Diseño muleta Erjuli")

### Construcción:
La construcción es sencilla, teniendo en cuenta el conexinado con la _shiel Zumo_ y las piezas impresas en 3D. En el programa está la relación de conexión de los pines, que se refleja en el siguiente esquema:

![Esquema conexión Erjuli](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/EsquemaConexion.JPG "Esquema conexionado Erjuli")

Es necesario desconectar el _juper_ de medición de baterías de la _shield_, para utilizar el pin A1 para el _echo_ de uno de los sensores. Lo mismo con los puentes de **I2C** para gestión del giróscopo, acelerómetro y brújula, que dejan libres A4 y A5 para el mismo menester.

En la páctica compensa utilizar un protoboard tipo para instalar los sensores **HC-SR04** y el conector del microservo **SG90**, esto dará mayor fiabilidad a las conexiones y la fijación de los elementos físicos, que sufirán mucho en el combate

![Esquema Zumo Shield](https://github.com/NacioSystems/Tseo/blob/master/Imagenes/Esquema%20Zumo.JPG "Esquema de la Zumo Shield")

La montera es opcional, se puede utilizar para el paseillo inicial, y dejarla fuera del _Dohyo_ durante el combate. Hay que tener en cuenta que la pérdia de una pieza de más de 5gr te puede dejar eliminado. Otra opción es pegar la montera a la cubierta de la carcasa con pegamento Imedio o similar, asegurando que no se perderán piezas durante la partida.

### Piezas impresas:

En la carpeta de piezas impresas están los modelos STL de la carcasa de _Erjuli_ donde se albergarán los senores **HC-SR04**, y el microservo. También están la montera, opcional, se puede llevar sólo para el paseíllo inicial, y el soporte que acopla el microservo al mástil de la muleta. 

![Carcasa Tseo](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Carcasa%20Erjuli.JPG "Carcasa Tseo")

También se incluyen los ficheros FreeCad donde está el diseño de estas piezas.

![Erjuli con montera](https://github.com/NacioSystems/Erjuli/blob/master/Imagenes/Carcasa%20con%20Montera.JPG "Erjuli con montera")


### Referencias:

* *[Página web OSHWDEM**][7]*
* *[Página Web reglamento Minisumo **OSHWDEM**][1]*
* *[Pololu Zumo for Arduino][2]*
* *[Arduino UNO R3][3]* 
* *[Pololu Zumo Reflectance Sensor Array][4]*
* *[Sensor de distancia HC-SR04 en BricoGeek][5]*
* *[Microservo SG90 en BricoGeek][6]*


### Autor:

**Ignacio Otero**



### Licencia:

Todos estos productos están liberados mediante Creative Commons Attribution-ShareAlike 4.0 International License.


[1]:http://rules.oshwdem.org/loita_sumo_es

[2]:https://www.pololu.com/product/2506
   
[3]:https://store.arduino.cc/arduino-uno-rev3

[4]:https://www.pololu.com/product/1419

[5]:https://tienda.bricogeek.com/sensores-distancia/741-sensor-de-distancia-por-ultrasonidos-hc-sr04.html

[6]:https://tienda.bricogeek.com/servomotores/968-micro-servo-miniatura-sg90.html

[7]:http://rules.oshwdem.org
