# TP_TDV

**1. (1 punto) Estrategia FCFS.** Implementar la solución que se corresponde con la estrategia
FCFS. Para ello, completar la clase GreedySolver, que toma una instancia y resuelve el
problema con la estrategia FCFS. Esta clase debe guardar la información respecto al valor
de la función objetivo, tiempo de resolución, y la solución a implementar usando la clase
TaxiAssignmentSolution, ya implementada.

**2. (1 puntos) Modelo para estrategia de batching.** Modelar el problema usando grafos,
incluyendo todos los parámetros y definiciones necesarias. Justificar la elección del mismo
y por qué lo consideran apropiado para el problema.

**3. (2 puntos) Implementación estrategia de batching.** Realizar una implementación del
modelo propuesto en el ítem anterior. Para ello, completar la clase MinCostFlowSolver,
con un comportamiento análogo a GreedySolver. Detalles sobre cómo realizar esta
implemtanción serán dados más adelante.

**4. (1 punto) Experimentación.** Realizar experimentos sobre todas las instancias comparando
el modelo propuesto respecto al de la solución actual de la compañía. En cada caso, medir la
mejora porcentual obtenida. Sean zb y zg el valor de la función objetivo de una solución del
modelo para el batching y el de FCFS, respectivamente. definimos la mejora relativa como
$$ %gap = \frac{z_g - z_b}{z_b}$$
En caso de considerarlo conveniente, pueden agregar otras métricas complementarias (respecto
al método o a las soluciones) para el análisis de los resultados.
Sugerencia: se recomienda sistematizar la realización de experimentos, idealmente definiendo
la lista de instancias a considerar y reportando en algún formato conveniente (por ejemplo, csv)
el resumen de los resultados obtenidos, para ser analizados posteriormente.

**5. (2 puntos) Discusión y análisis de resultados.** Analizar los resultados obtenidos en
función de la comparación entre ambos métodos. Incluir como parte de la discusión aquello
que considere relevante en relación a las características del modelo propuesto para resolver
el problema (por ejemplo, tiempo de ejecución vs. contexto de aplicación, posibles adaptaciones
ante limitantes, etc.).

**6. (1 punto) Limitaciones y posibles extensiones.** El nuevo modelo propuesto puede no
considerar algunos aspectos importantes a la hora de implementarlo. En base a encuestas
realizadas a los conductores en general, las aplicaciones sugieren muchas veces viajes que
demandan una distancia considerable para llegar a la ubicación del pasajero, para luego
realizar un muy viaje corto en comparación. En este sentido, la sensación de los conductores
es que es mucho el costo de buscar el pasajero, ya sea en costo específico o en el tiempo
utilizado y que podrían destinar a un viaje más rentable, en relación al beneficio obtenido
por el viaje en sí mismo. Se pide analizar las soluciones de los métodos propuestos utilizando
las distancias paticulares codificadas en la solución con la información adicional disponible
de cada pasajero. Proponer alguna modificación a los modelos para abordar la problemática
identificada.

**7. (2 puntos) Modelo alternativo.** Implementar la mejora propuesta y comparar los nuevos
resultados con los obtenidos originalmente en términos de la distancia total recorrida. De
ser posible, medir el impacto.
