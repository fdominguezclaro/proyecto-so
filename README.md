# proyecto-so

## Integrantes

- Paul Heinsohn: 1562305J
- Manuel Aguirre: 15624005
- Jacques Hasard: 1562188J
- Felipe Dominguez: 1562157J

## Principales decisiones de diseño
* En la mayoría de las funciones de nuestra API se carga un árbol de directorios que permite el recorrido de estos tanto en formato de DFS como de BFS, así es posible desplazarse entre las carpetas de manera más amena y poder tanto buscar como encontrar directorios con el fin de trabajar sobre ellos.
* Los bloques de índices y de directorios válidos en el disco fueron asignados a estructuras personalizadas por nosotros vía `typedef` y guardados en memoria. El bitmap es un arreglo de caracteres guardado en memoria, mientras que los bloques de direccionamiento indirecto y de datos se representan mediante punteros de índices para cada archivo existente que desee abrirse.

## Supuestos
* Al momento de hacer `cr_open()` en modo 'w' a un archivo existente, se le asignará un nuevo bloque de memoria a dicho hardlink en caso de ser posible, por lo que dejará de apuntar a los bloques de memoria que apuntaba antes (la cantidad de hardlinks del original se modifica acorde a esto). Se tomó en cuenta basándose en `open()` de Python.
* Como guardamos las partes necesarias a emplear según el comando en memoria, nuestro `cr_close()` solo debe liberarla, ya que `cr_write()` se encarga de que cualquier escritura se efectúe a tiempo.

## Bonus
* Al momento de abrir archivos con `cr_open()` en modo 'w' inexistentes, se crearan... incluso si deberán crearse nuevos directorios. Lo mismo ocurre con `cr_mkdir()`.
* Se creó una documentación de la API llamada 'DOCUMENTACION.md'.
* Se manejan los errores en base a errno y a nuestras estimaciones... Mayor detalle en la documentación.
