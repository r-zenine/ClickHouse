---
machine_translated: true
---

# Copia de seguridad de datos {#data-backup}

Mientras [replicación](table_engines/replication.md) proporciona protección contra fallas de hardware, no protege contra errores humanos: eliminación accidental de datos, eliminación de la tabla incorrecta o una tabla en el clúster incorrecto y errores de software que resultan en un procesamiento incorrecto de datos o daños en los datos. En muchos casos, errores como estos afectarán a todas las réplicas. ClickHouse tiene protecciones integradas para evitar algunos tipos de errores, por ejemplo, de forma predeterminada [no puede simplemente colocar tablas con un motor similar a MergeTree que contenga más de 50 Gb de datos](https://github.com/ClickHouse/ClickHouse/blob/v18.14.18-stable/dbms/programs/server/config.xml#L322-L330). Sin embargo, estas garantías no cubren todos los casos posibles y pueden eludirse.

Para mitigar eficazmente los posibles errores humanos, debe preparar cuidadosamente una estrategia para realizar copias de seguridad y restaurar sus datos **previamente**.

Cada empresa tiene diferentes recursos disponibles y requisitos comerciales, por lo que no existe una solución universal para las copias de seguridad y restauraciones de ClickHouse que se adapten a cada situación. Lo que funciona para un gigabyte de datos probablemente no funcionará para decenas de petabytes. Hay una variedad de posibles enfoques con sus propios pros y contras, que se discutirán a continuación. Es una buena idea utilizar varios enfoques en lugar de solo uno para compensar sus diversas deficiencias.

!!! note "Nota"
    Tenga en cuenta que si realizó una copia de seguridad de algo y nunca intentó restaurarlo, es probable que la restauración no funcione correctamente cuando realmente la necesite (o al menos tomará más tiempo de lo que las empresas pueden tolerar). Por lo tanto, cualquiera que sea el enfoque de copia de seguridad que elija, asegúrese de automatizar el proceso de restauración también y practicarlo en un clúster de ClickHouse de repuesto regularmente.

## Duplicar datos de origen en otro lugar {#duplicating-source-data-somewhere-else}

A menudo, los datos que se ingieren en ClickHouse se entregan a través de algún tipo de cola persistente, como [Acerca de nosotros](https://kafka.apache.org). En este caso, es posible configurar un conjunto adicional de suscriptores que leerá el mismo flujo de datos mientras se escribe en ClickHouse y lo almacenará en almacenamiento en frío en algún lugar. La mayoría de las empresas ya tienen algún almacenamiento en frío recomendado por defecto, que podría ser un almacén de objetos o un sistema de archivos distribuido como [HDFS](https://hadoop.apache.org/docs/stable/hadoop-project-dist/hadoop-hdfs/HdfsDesign.html).

## Instantáneas del sistema de archivos {#filesystem-snapshots}

Algunos sistemas de archivos locales proporcionan funcionalidad de instantánea (por ejemplo, [ZFS](https://en.wikipedia.org/wiki/ZFS)), pero podrían no ser la mejor opción para servir consultas en vivo. Una posible solución es crear réplicas adicionales con este tipo de sistema de archivos y excluirlas del [Distribuido](table_engines/distributed.md) tablas que se utilizan para `SELECT` consulta. Las instantáneas en tales réplicas estarán fuera del alcance de cualquier consulta que modifique los datos. Como beneficio adicional, estas réplicas podrían tener configuraciones de hardware especiales con más discos conectados por servidor, lo que sería rentable.

## Método de codificación de datos: {#clickhouse-copier}

[Método de codificación de datos:](utils/clickhouse-copier.md) es una herramienta versátil que se creó inicialmente para volver a dividir tablas de tamaño petabyte. También se puede usar con fines de copia de seguridad y restauración porque copia datos de forma fiable entre tablas y clústeres de ClickHouse.

Para volúmenes de datos más pequeños, un simple `INSERT INTO ... SELECT ...` a tablas remotas podría funcionar también.

## Manipulaciones con piezas {#manipulations-with-parts}

Haz clickHouse permite usar el `ALTER TABLE ... FREEZE PARTITION ...` consulta para crear una copia local de particiones de tabla. Esto se implementa utilizando enlaces duros al `/var/lib/clickhouse/shadow/` carpeta, por lo que generalmente no consume espacio adicional en disco para datos antiguos. Las copias creadas de archivos no son manejadas por el servidor ClickHouse, por lo que puede dejarlos allí: tendrá una copia de seguridad simple que no requiere ningún sistema externo adicional, pero aún así será propenso a problemas de hardware. Por esta razón, es mejor copiarlos de forma remota en otra ubicación y luego eliminar las copias locales. Los sistemas de archivos distribuidos y los almacenes de objetos siguen siendo una buena opción para esto, pero los servidores de archivos conectados normales con una capacidad lo suficientemente grande podrían funcionar también (en este caso, la transferencia ocurrirá a través del sistema de archivos de red o tal vez [rsync](https://en.wikipedia.org/wiki/Rsync)).

Para obtener más información sobre las consultas relacionadas con las manipulaciones de particiones, consulte [Documentación de ALTER](../query_language/alter.md#alter_manipulations-with-partitions).

Una herramienta de terceros está disponible para automatizar este enfoque: [Haga clic en el botón de copia de seguridad](https://github.com/AlexAkulov/clickhouse-backup).

[Artículo Original](https://clickhouse.tech/docs/es/operations/backup/) <!--hide-->
