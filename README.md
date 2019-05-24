# proyecto-so

## Integrantes

- Paul Heinsohn: 1562305J
- Manuel Aguirre: 15624005
- Jacques Hasard: 1562188J
- Felipe Dominguez: 1562157J

## Principales decisiones de diseño

## Supuestos
* Tanto los Direct Blocks como los Data Blocks se implementaron mediante punteros de índices, ya que estimamos que asignarles una estructura con typedef era innecesario.
* Al momento de abrir archivos con cr_open() en modo 'w' inexistentes, se crearan... incluso si deberán crearse nuevos directorios. Lo mismo ocurre con cr_mkdir().
* Al momento de hacer cr_open() en modo 'w' a un archivo existente, se le asignará un nuevo bloque de memoria a dicho hardlink en caso de ser posible, por lo que dejará de apuntar a los bloques de memoria que apuntaba antes. Se tomó en cuenta basándose en open() de Python.
* Como guardamos las partes necesarias a emplear según el comando en memoria, nuestro cr_close() solo debe liberarla, ya que cr_write() se encarga de que cualquier escritura se efectúe a tiempo.
