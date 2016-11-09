# 1. Предварительная архитектура СНС

## Архитектура НС состоит из классов:

1. Класс ConvNetwork
2. класс слоя -- class ConvLayer
3. class ConvNeuron -- нейрон сверточной НС
4. Вспомогательные классы:
	* class FileInterface -> .h
	* class VpsInterface -> other prg
	* class Log -> .h

# 2. class ConvNeuron
## Methods

```
FRIEND:
1. friend layer::writeNeuronWeights(FILE *fp)

PUBLIC:
0. ConvNeuron(bool isRand = false)
1. processMap(vector <vector<double>> &map, ...)
2. initNeuron(double alpha, ..

PRIVATE:
1. randomizeCore(...)
2. tFunc(...) -- threshold function

3. writeCore(FILE *fp)
3.1 readCore(FILE *fp)

4. getCore
5. changeCore

6. getActWeight
7. changeActWeight
```

## Data

```
PRIVATE:
1. vector <vector<double>> featureMap -- карта признаков -- выход нейрона
2. vector <vector<double>> convCore -- ядро свертки для нейрона
3. double aWeight -- activational weight
5. int inputMapHeight...
6. bool randomize
PUBLIC:
6. Any other needed variables
```
