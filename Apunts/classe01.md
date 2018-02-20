# IDI
> Professora: Marta Fairén

# Introducció:
```
Dades -> Model -> Visualització -> Perifèric
```

### Model:
`Sòlids -> Model de fronteres` aproximació per cares planes: **triangles**

### Visualització:
`De 3D a 2D`
Des d'on vull que es vegi "càmera"
`Mode` Com ho vull pintar

`Llums`

|Dades|Model|Visualització (Open GL 3.3)|Perifèric|
|-|-|-|-|
|-|Triangles|Càmera + Mode + Llums|Píxels|

## Elements d'un sistema gràfic:
### Sortida:
`Ens centrem en pantalles d'escombrat/raster:` S'esborra 60 vegades per segon: **60Hz**

*Va de la cantonada inferior esquerra a la superior dreta*

#### Píxel:
> Unitat mínima a la que li puc donar un color.
> ```
> píxel (x,y,c)
> ```
> **Resolució X:** nº de píxels en horitzontal `(0..res_x-1)`
>
> **Resolució Y:** nº de píxels en vertical `(0..res_y-1)`
>
> ***Permet identificar els píxels de forma única.***

#### Síntesi additiva del color:
`Model RGB` (Red-Green-Blue)

|RED|GREEN|BLUE|Resultat|
|-|-|-|-|
|0|0|0|negre|
|1|0|0|vermell|
|0|1|0|verd|
|0|0|1|blau|
|0|1|1|cian|
|1|0|1|magenta|
|1|1|0|groc|
|1|1|1|blanc|

*Necessitem més tonalitats, adaptem la intensitat (0..1) -> (0..255), obtenint així gairebé 16 milions de color.*

```
c = (r,g,b)

per exemple:
r = 0.5 -> meitat de la intensitat
r = 0.8 -> s'apropa més al màxim
```

#### Frame buffer:
Tros de memòria de la tarja gràfica on hi guardem la informació.

> **Flickering:**
>
> Si s'esborra una part del frame mentre a l'altre s'hi escriu es produeix un tremolor.
>
> *Solució:* Doblar el buffer i anar alternant entre un i altre.
>
> **Aliasing:**
>
> Efecte d'escaleta quan els píxels són grossos. O amb línies molt rectes.
>
> *Solució:* Emborronar l'aliasing.

#### Finestra gràfica:
- Finestra de l'aplicació
- Finestra OpenGL ("pantalla")
---

## Models Geomètrics:

`Necessitem sistema de coordenades` per guardar-ho (de la mà dreta).

`PUNT:` P(x,y,z)

> x \* y -> z
>
> x \* z -> y
>
> y \* z -> x

*Apliquem el model de fronteres.* -> No sabem com formar els triangles.

**Necessitem informació de les arestes!**

Si coneixem els 3 vèrtexs d'un triangle:
- Geometria
- Topologia (implicitament)

-> Es repeteixen els 3 vèrtexs!

**Solució: Taula de cares + taula de vèrtexs**

|normal|ID vèrtex|
|:-:|:-:|
|a1,b1,c1|1,2,3|
|a2,b2,c2|2,4,3|

|X|Y|Z
|:-:|-:|-:|
|-1|1|-1|
|-1|1|1|
|1|1|1|
|1|-1|1|

### Model de fronteres vàlid:
- Cares "orientades" (on és fora vs. dins).
- Ordenació vèrtexs coherent amb ordenació de les cares (regla de la mà dreta).
> El donem en sentit antihorari -> La normal va cap a fora de l'objecte. En cas contrari, estem a l'interior
- Cada aresta separa 2 cares.

## Pintar en OpenGL:

> Pintar un sol cop a la GPU/OpenGL

*Necessitarem estructura auxiliar amb repetició de vèrtexs*

1. Creem un VAO i un VBO. (una vegada).

  > `VAO:` Encapsula les dades del model. ("carpeta")
  >
  > `VBO:` Conté les coordenades (dades) del model. ("arxius en carpeta")

2. Guardar la llista de vèrtexs (amb repetició) en el VBO. (una vegada).
3. Cada cop que es requereix pintar, indiquem el VAO a pintar i cridem a la funció. (més d'una vegada).

```
glDrawArrays(...)

pinta_model()
```
