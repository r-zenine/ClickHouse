---
machine_translated: true
---

# Funciones de mapa de bits {#bitmap-functions}

Las funciones de mapa de bits funcionan para dos mapas de bits Cálculo del valor del objeto, es devolver un nuevo mapa de bits o cardinalidad mientras se usa el cálculo de la fórmula, como y, o, xor, y no, etc.

Hay 2 tipos de métodos de construcción para Bitmap Object. Uno debe ser construido por la función de agregación groupBitmap con -State , el otro debe ser construido por Array Object . También es para convertir objeto de mapa de bits a objeto de matriz.

RoaringBitmap se envuelve en una estructura de datos, mientras que el almacenamiento real de los objetos de mapa de bits. Cuando la cardinalidad es menor o igual que 32, utiliza Set objet. Cuando la cardinalidad es mayor que 32, utiliza el objeto RoaringBitmap. Es por eso que el almacenamiento del conjunto de baja cardinalidad es más rápido.

Para obtener más información sobre RoaringBitmap, consulte: [CRoaring](https://github.com/RoaringBitmap/CRoaring).

## bitmapBuild {#bitmap_functions-bitmapbuild}

Construya un mapa de bits a partir de una matriz de enteros sin signo.

``` sql
bitmapBuild(array)
```

**Parámetros**

-   `array` – matriz entera sin signo.

**Ejemplo**

``` sql
SELECT bitmapBuild([1, 2, 3, 4, 5]) AS res, toTypeName(res)
```

``` text
┌─res─┬─toTypeName(bitmapBuild([1, 2, 3, 4, 5]))─────┐
│     │ AggregateFunction(groupBitmap, UInt8)    │
└─────┴──────────────────────────────────────────────┘
```

## bitmapToArray {#bitmaptoarray}

Convertir mapa de bits a matriz entera.

``` sql
bitmapToArray(bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapBuild([1, 2, 3, 4, 5])) AS res
```

``` text
┌─res─────────┐
│ [1,2,3,4,5] │
└─────────────┘
```

## bitmapSubsetInRange {#bitmap-functions-bitmapsubsetinrange}

Devuelve el subconjunto en el rango especificado (no incluye range\_end).

``` sql
bitmapSubsetInRange(bitmap, range_start, range_end)
```

**Parámetros**

-   `bitmap` – [Objeto de mapa de bits](#bitmap_functions-bitmapbuild).
-   `range_start` – punto de inicio de la gama. Tipo: [UInt32](../../data_types/int_uint.md).
-   `range_end` – punto final de rango(excluido). Tipo: [UInt32](../../data_types/int_uint.md).

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapSubsetInRange(bitmapBuild([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,100,200,500]), toUInt32(30), toUInt32(200))) AS res
```

``` text
┌─res───────────────┐
│ [30,31,32,33,100] │
└───────────────────┘
```

## bitmapSubsetLimit {#bitmapsubsetlimit}

Crea un subconjunto de mapa de bits con n elementos tomados entre `range_start` y `cardinality_limit`.

**Sintaxis**

``` sql
bitmapSubsetLimit(bitmap, range_start, cardinality_limit)
```

**Parámetros**

-   `bitmap` – [Objeto de mapa de bits](#bitmap_functions-bitmapbuild).
-   `range_start` – El punto de partida del subconjunto. Tipo: [UInt32](../../data_types/int_uint.md).
-   `cardinality_limit` – El subconjunto cardinalidad límite superior. Tipo: [UInt32](../../data_types/int_uint.md).

**Valor devuelto**

Subconjunto.

Tipo: `Bitmap object`.

**Ejemplo**

Consulta:

``` sql
SELECT bitmapToArray(bitmapSubsetLimit(bitmapBuild([0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,100,200,500]), toUInt32(30), toUInt32(200))) AS res
```

Resultado:

``` text
┌─res───────────────────────┐
│ [30,31,32,33,100,200,500] │
└───────────────────────────┘
```

## bitmapContains {#bitmap_functions-bitmapcontains}

Comprueba si el mapa de bits contiene un elemento.

``` sql
bitmapContains(haystack, needle)
```

**Parámetros**

-   `haystack` – [Objeto de mapa de bits](#bitmap_functions-bitmapbuild), donde la función busca.
-   `needle` – Valor que busca la función. Tipo: [UInt32](../../data_types/int_uint.md).

**Valores devueltos**

-   0 — Si `haystack` no contiene `needle`.
-   1 — Si `haystack` contener `needle`.

Tipo: `UInt8`.

**Ejemplo**

``` sql
SELECT bitmapContains(bitmapBuild([1,5,7,9]), toUInt32(9)) AS res
```

``` text
┌─res─┐
│  1  │
└─────┘
```

## bitmapHasAny {#bitmaphasany}

Comprueba si dos mapas de bits tienen intersección por algunos elementos.

``` sql
bitmapHasAny(bitmap1, bitmap2)
```

Si está seguro de que `bitmap2` Esta es la página de desambiguación en 24symbols. [bitmapContains](#bitmap_functions-bitmapcontains) función. Funciona de manera más eficiente.

**Parámetros**

-   `bitmap*` – objeto de mapa de bits.

**Valores de retorno**

-   `1` si `bitmap1` y `bitmap2` tienen un elemento similar al menos.
-   `0`, de lo contrario.

**Ejemplo**

``` sql
SELECT bitmapHasAny(bitmapBuild([1,2,3]),bitmapBuild([3,4,5])) AS res
```

``` text
┌─res─┐
│  1  │
└─────┘
```

## bitmapHasAll {#bitmaphasall}

Análogo a `hasAll(array, array)` devuelve 1 si el primer mapa de bits contiene todos los elementos del segundo, 0 de lo contrario.
Si el segundo argumento es un mapa de bits vacío, devuelve 1.

``` sql
bitmapHasAll(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapHasAll(bitmapBuild([1,2,3]),bitmapBuild([3,4,5])) AS res
```

``` text
┌─res─┐
│  0  │
└─────┘
```

## bitmapCardinalidad {#bitmapcardinality}

Vuelva a ejecutar la cardinalidad del mapa de bits de tipo UInt64.

``` sql
bitmapCardinality(bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapCardinality(bitmapBuild([1, 2, 3, 4, 5])) AS res
```

``` text
┌─res─┐
│   5 │
└─────┘
```

## bitmapMin {#bitmapmin}

Vuelva a ejecutar el valor más pequeño de tipo UInt64 en el conjunto, UINT32\_MAX si el conjunto está vacío.

    bitmapMin(bitmap)

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapMin(bitmapBuild([1, 2, 3, 4, 5])) AS res
```

    ┌─res─┐
    │   1 │
    └─────┘

## bitmapMax {#bitmapmax}

Vuelva a ejecutar el mayor valor de tipo UInt64 en el conjunto, 0 si el conjunto está vacío.

    bitmapMax(bitmap)

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapMax(bitmapBuild([1, 2, 3, 4, 5])) AS res
```

    ┌─res─┐
    │   5 │
    └─────┘

## bitmapTransform {#bitmaptransform}

Transformar una matriz de valores en un mapa de bits a otra matriz de valores, el resultado es un nuevo mapa de bits.

    bitmapTransform(bitmap, from_array, to_array)

**Parámetros**

-   `bitmap` – objeto de mapa de bits.
-   `from_array` – matriz UInt32. Para idx en el rango \[0, from\_array .size()), si el mapa de bits contiene from\_array\[idx\] , luego reemplácelo con to\_array\[idx\] . Tenga en cuenta que el resultado depende del orden de la matriz si hay elementos comunes entre from\_array y to\_array .
-   `to_array` – Matriz UInt32, su tamaño será el mismo que from\_array.

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapTransform(bitmapBuild([1, 2, 3, 4, 5, 6, 7, 8, 9, 10]), cast([5,999,2] as Array(UInt32)), cast([2,888,20] as Array(UInt32)))) AS res
```

    ┌─res───────────────────┐
    │ [1,3,4,6,7,8,9,10,20] │
    └───────────────────────┘

## bitmapAnd {#bitmapand}

Dos mapa de bits y cálculo, el resultado es un nuevo mapa de bits.

``` sql
bitmapAnd(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapAnd(bitmapBuild([1,2,3]),bitmapBuild([3,4,5]))) AS res
```

``` text
┌─res─┐
│ [3] │
└─────┘
```

## bitmapO {#bitmapor}

Dos mapa de bits o cálculo, el resultado es un nuevo mapa de bits.

``` sql
bitmapOr(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapOr(bitmapBuild([1,2,3]),bitmapBuild([3,4,5]))) AS res
```

``` text
┌─res─────────┐
│ [1,2,3,4,5] │
└─────────────┘
```

## bitmapXor {#bitmapxor}

Dos bitmap xor cálculo, el resultado es un nuevo mapa de bits.

``` sql
bitmapXor(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapXor(bitmapBuild([1,2,3]),bitmapBuild([3,4,5]))) AS res
```

``` text
┌─res───────┐
│ [1,2,4,5] │
└───────────┘
```

## bitmapAndnot {#bitmapandnot}

Dos mapas de bits y no cálculo, el resultado es un nuevo mapa de bits.

``` sql
bitmapAndnot(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapToArray(bitmapAndnot(bitmapBuild([1,2,3]),bitmapBuild([3,4,5]))) AS res
```

``` text
┌─res───┐
│ [1,2] │
└───────┘
```

## Bienvenido a WordPress. {#bitmapandcardinality}

Dos mapa de bits y cálculo, devuelven cardinalidad de tipo UInt64.

``` sql
bitmapAndCardinality(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapAndCardinality(bitmapBuild([1,2,3]),bitmapBuild([3,4,5])) AS res;
```

``` text
┌─res─┐
│   1 │
└─────┘
```

## bitmapOrCardinalidad {#bitmaporcardinality}

Dos mapa de bits o cálculo, retorno cardinalidad de tipo UInt64.

``` sql
bitmapOrCardinality(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapOrCardinality(bitmapBuild([1,2,3]),bitmapBuild([3,4,5])) AS res;
```

``` text
┌─res─┐
│   5 │
└─────┘
```

## bitmapXorCardinalidad {#bitmapxorcardinality}

Dos bitmap xor cálculo, retorno cardinalidad de tipo UInt64.

``` sql
bitmapXorCardinality(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapXorCardinality(bitmapBuild([1,2,3]),bitmapBuild([3,4,5])) AS res;
```

``` text
┌─res─┐
│   4 │
└─────┘
```

## Por favor, introduzca su dirección de correo electrónico {#bitmapandnotcardinality}

Dos mapas de bits yno cálculo, devuelve cardinalidad de tipo UInt64.

``` sql
bitmapAndnotCardinality(bitmap,bitmap)
```

**Parámetros**

-   `bitmap` – objeto de mapa de bits.

**Ejemplo**

``` sql
SELECT bitmapAndnotCardinality(bitmapBuild([1,2,3]),bitmapBuild([3,4,5])) AS res;
```

``` text
┌─res─┐
│   2 │
└─────┘
```

[Artículo Original](https://clickhouse.tech/docs/es/query_language/functions/bitmap_functions/) <!--hide-->
