---
machine_translated: true
---

# MySQL {#mysql}

El motor MySQL le permite realizar `SELECT` Consultas sobre datos almacenados en un servidor MySQL remoto.

## Creación de una tabla {#creating-a-table}

``` sql
CREATE TABLE [IF NOT EXISTS] [db.]table_name [ON CLUSTER cluster]
(
    name1 [type1] [DEFAULT|MATERIALIZED|ALIAS expr1] [TTL expr1],
    name2 [type2] [DEFAULT|MATERIALIZED|ALIAS expr2] [TTL expr2],
    ...
) ENGINE = MySQL('host:port', 'database', 'table', 'user', 'password'[, replace_query, 'on_duplicate_clause']);
```

Vea una descripción detallada del [CREAR TABLA](../../query_language/create.md#create-table-query) consulta.

La estructura de la tabla puede diferir de la estructura de la tabla MySQL original:

-   Los nombres de columna deben ser los mismos que en la tabla MySQL original, pero puede usar solo algunas de estas columnas y en cualquier orden.
-   Los tipos de columna pueden diferir de los de la tabla MySQL original. Haga clic en Casa intenta [elenco](../../query_language/functions/type_conversion_functions.md#type_conversion_function-cast) valores a los tipos de datos ClickHouse.

**Parámetros del motor**

-   `host:port` — Dirección del servidor MySQL.

-   `database` — Nombre de base de datos remota.

-   `table` — Nombre de la tabla remota.

-   `user` — Usuario de MySQL.

-   `password` — Contraseña de usuario.

-   `replace_query` — Bandera que convierte `INSERT INTO` Consultas a `REPLACE INTO`. Si `replace_query=1`, la consulta se sustituye.

-   `on_duplicate_clause` — El `ON DUPLICATE KEY on_duplicate_clause` expresión que se añade a la `INSERT` consulta.

    Ejemplo: `INSERT INTO t (c1,c2) VALUES ('a', 2) ON DUPLICATE KEY UPDATE c2 = c2 + 1`, donde `on_duplicate_clause` ser `UPDATE c2 = c2 + 1`. Ver el [Documentación de MySQL](https://dev.mysql.com/doc/refman/8.0/en/insert-on-duplicate.html) para encontrar qué `on_duplicate_clause` se puede utilizar con el `ON DUPLICATE KEY` clausula.

    Especificar `on_duplicate_clause` tienes que pasar `0` Angeles `replace_query` parámetro. Si pasa simultáneamente `replace_query = 1` y `on_duplicate_clause`, ClickHouse genera una excepción.

Simple `WHERE` cláusulas tales como `=, !=, >, >=, <, <=` se ejecutan en el servidor MySQL.

El resto de las condiciones y el `LIMIT` La restricción de muestreo se ejecuta en ClickHouse solo después de que finalice la consulta a MySQL.

## Ejemplo de uso {#usage-example}

Tabla en MySQL:

``` text
mysql> CREATE TABLE `test`.`test` (
    ->   `int_id` INT NOT NULL AUTO_INCREMENT,
    ->   `int_nullable` INT NULL DEFAULT NULL,
    ->   `float` FLOAT NOT NULL,
    ->   `float_nullable` FLOAT NULL DEFAULT NULL,
    ->   PRIMARY KEY (`int_id`));
Query OK, 0 rows affected (0,09 sec)

mysql> insert into test (`int_id`, `float`) VALUES (1,2);
Query OK, 1 row affected (0,00 sec)

mysql> select * from test;
+--------+--------------+-------+----------------+
| int_id | int_nullable | float | float_nullable |
+--------+--------------+-------+----------------+
|      1 |         NULL |     2 |           NULL |
+--------+--------------+-------+----------------+
1 row in set (0,00 sec)
```

Tabla en ClickHouse, recuperando datos de la tabla MySQL creada anteriormente:

``` sql
CREATE TABLE mysql_table
(
    `float_nullable` Nullable(Float32),
    `int_id` Int32
)
ENGINE = MySQL('localhost:3306', 'test', 'test', 'bayonet', '123')
```

``` sql
SELECT * FROM mysql_table
```

``` text
┌─float_nullable─┬─int_id─┐
│           ᴺᵁᴸᴸ │      1 │
└────────────────┴────────┘
```

## Ver también {#see-also}

-   [El ‘mysql’ función de la tabla](../../query_language/table_functions/mysql.md)
-   [Uso de MySQL como fuente de diccionario externo](../../query_language/dicts/external_dicts_dict_sources.md#dicts-external_dicts_dict_sources-mysql)

[Artículo Original](https://clickhouse.tech/docs/es/operations/table_engines/mysql/) <!--hide-->
