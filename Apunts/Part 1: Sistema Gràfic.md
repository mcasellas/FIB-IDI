# IDI

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

----

# Models Geomètrics:

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

-----

## Càmera:

### Elements:

- **OBS** (observador)
- **VRP** (view reference point)
- **Up** (view up vector) *Inclinació de la càmera -> direcció eix vertical*

*Eixos de l'observador* **des de dalt:**

-|-|-
-|-|-
-|Y|X
-|Z|-

### Matriu:

s.x|s.y|s.z|0
-|-|-|-
w.x|w.y|w.z|0
F.x|F.y|F.z|0
0|0|0|1

```
  VM = Matriu * Trans(-OBS)
```
*La matriu la calculem:*
```
  F = OBS-VRP=(F.x,F.y,F.z)   F = F / ||F||

  S = up x F

  w = F x s
```

### Òptica:

> Varia la projecció.
>
> Ambdòs tenen window.


#### - Perspectiva:
Els punts més llunyans es veuen més petits.
```
   ⍺v (FOV = 2⍺v), RAw (relació aspecte), zNear, zFar
```

#### - Octogonal:
Els punts llunyants es veuen a la mateixa mida.
```
  l,r,b,t, -window-, zNear, zFar

  on left, right, bottom, top
```

### Relació d'aspecte del *window* i del *viewport*:

No deformació: `RAw = Rav`

***Exemple 1:*** Càmera perspectiva

Posició i orientació:
```
  Base: (0,10,0)
  Cap: (0,25,0)
  Con: (2.5, 25,0), r=2 i llargada 5 orientat segons l'eix X+

  VRP = (0,15,0) // Centre del ninot
  OBS = (30,15,0) // Preservo la Y i la X
  UP = (0,1,0)
```
Òptica i perspectiva:
```
  zNear = 20 // Estic a 30 i el ninot a 10 -> 30-10 = 20
  zFar = 40 // Començo a 20 i mesura 20 -> 20+20 = 40

  30/2 = 15 -> ⍺ = arctg(15/20) = 36,8º
  RAw = 20/30 = 0,66667 // Deformació

  // Arreglem
  RAv = 1
```

### Optenir punts en coordenades de clipping:

#### Capsa de l'escena:
> Calcular els paràmetres de la **càmera**.

#### Capsa del model:
> Calcular les **transformacions geomètriques** a aplicar al model.


Vup != V
