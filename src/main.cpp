/*
==================================================================
Projeto: JSON
Descrição: Atividade JSON
Autor: Rafael Ribeiro Gonçalves
Data: 21/10/2026
Versão: 0.1
==================================================================
*/

#include <Arduino.h>
#include <ArduinoJson.h>

void setup() 
{
  Serial.begin(115200);
  // raw string literal (string literal "crua") -> R"(texto aqui)"
  const char* resposta = R"(
    {
      "nome" : "Rafael",
      "idade" : "19",
      "naturalidade" : "Sao Paulo/SP",
      "isSingle" : true,
      "time" : "Santos",
      "temperatura" : 37,
      "hobbies" : ["academia", "jogos", "esportes"],
      "endereco" :{
        "rua" : "Niteroi",
        "numero" : "180",
        "coordenadas" :{
          "lat" : -23.62,
          "lon" : -46.55
          }
      }
    }
  )";

  //* DeserializationError eh um tipo, uma clase do ArduinoJson que guarda um codigo
  //* Informa que o parse (segmentacao de chave valor) terminou  ||| O parse separa as palavras
  JsonDocument doc;

  DeserializationError erroJson = deserializeJson(doc, resposta);
  
  //*Possiveis erros
  //* 0 ou OK -----> SUCESSO
  //* 1 ou EmptyInput -----> ENTRADA VAZIO OU TIMEOUT DA STREAM
  //* 2 ou IncompleteInput -----> JSON CORTADO ANTES DE TERMINAR
  //* 3 ou InvalidInput -----> SINTAXE/FORMATO INVALIDA
  //* 4 ou NoMemory -----> MEMORIA INSUFICIENTE PARA MONTAR O DOCUMENTO 
  //* 5 ou TooDeep -----> ALINHAMENTO ACIMA DO LIMITE DEFINIDO PELA FUNCAO 
  
  //*             NestingLimit DEFINIDO COMO PADRAO EM
  //*              ARDUINO_DEFAULT_NESTING_LIMIT = 10 CADA { ou [
  //*              ABERTO DENTRO DE OUTRO SOMA UM NIVEL 
  //*              EXEMPLO: {
  //*                         "a" : {
  //*                                 "b" : [1,2]
  //*                               }
  //*                        } TEM 3 NIVEIS
  switch(erroJson.code())
  {

    case DeserializationError::Ok: 
    
        break;

    case DeserializationError::EmptyInput: 
        Serial.println("JSON vazio ou expirou o tempo");
        return;

    case DeserializationError::IncompleteInput: 
        Serial.println("JSON incompleto");
        return;

    case DeserializationError::InvalidInput: 
        Serial.println("JSON mal formado"); 
        return;

    case DeserializationError::NoMemory: 
        Serial.println("Sem memoria");
        return;

    case DeserializationError::TooDeep:
        Serial.println("Alinhamento acima do limite definido pela funcao"); 
        return;
    
    default:
        Serial.println(erroJson.c_str());
        return;

  }

 /*
  //! Clausula de guarda, imprime caso JSON tenha algum erro
  if(erroJson)
  {
    Serial.printf("Erro: %s\n\r", erroJson.c_str());
    return;
  }
  */

  //* Verifica se existe a chave e se o tipo é adequado
  String nome = "";

  //*Confere se o conetudo da chave eh um texto
  if(doc["nome"].is<const char*>())
  nome = "Meu nome é " + doc["nome"].as<String>() + ", ";
  
  //* Verifica a existencia da chave, caso nao existir passasse um valor negativo definido
  //* para indicar a inexistencia, pois 0 neste caso eh um valor possivel
  //* o simbolo "|" (pipe) aqui nao eh um "OU bit a bit", a biblioteca redefinenesse operador
  //* para significar "Valor padrao", a logica eh se o valor da esquerda puder ser convertido
  //* para o dtipo da direita ele, usa ele; se nao, usa o da direita
  int idade = doc["idade"] | -1;
  
  //* Verificar a existencia da chave e um valor padrao caso nao existir
  String naturalidade = doc["naturalidade"] | "Nao informado";
  
  //*Normalmente uma variavel booleana tem como valor padrao tem como valor padrao false
  bool estadoCivil = doc["isSingle"] | false;
  
  String timeFutebol = doc ["time"];
  
  int temperatura;
  temperatura = 36;
  //* "is<int>" faz uma pergunta
  if(doc["temperatura"].is<int>()) //* a variavel so recebera um novo valor se a chave existir
  {
    //* retorna como alguma coisa
    temperatura = doc["temperatura"].as<int>();
  }

  //* =============================== TRATAMENTO DE VETORES ===============================
  const int tamanho = 10;
  const char* hobbies[10];
  size_t quantidade = 0;
  //* size_t eh um tipo inteiro usado para representar tamanho e quantidades
  //* de bytes de memoria, numeros de elementos e indices de vetor, eh o tipo que
  //* a funcao sizeof() retorna. Nao tem sinal (unsigned) e a largura depende
  //* depende da arquitetura mas tem o tamanho necessario para representar qualquer objeto
  //* na memoria da maquina em questao. 

  //! eh uma variavel que guarda uma memoria uint
  
  //* sizeof retorna o tamanho do vetor em bytes, porém cada posicao pode ter mais que um byte,
  //* por isso dividimos o tamanho total pelo tamanho de um item do vetor existente, 
  //* como sempre existe a posicao [0], utilizamos ela como padrao
  //* 
  //* sizeof() em printf usar %zu
  int tamanhoVetor = sizeof(hobbies) / sizeof(hobbies[0]);

  //* for range-based for loop | for baseadoo em intervalos
  //* informalmente  eh chamado de foreach (para cada) pq eh o nome
  //* do mesmo recurso em outras linguagens

  int numeros[] = {10, 20, 30, 40};

  //* Percorre uma lista onde nao sei a quantidade presente na lista
  for(int n : numeros)
  {
    Serial.println(n);
  }

  size_t tamanhoVetorExemplo = sizeof(numeros) / sizeof(numeros[0]);

  //* Sei a quantidade da lista
  for(int i = 0; i < tamanhoVetorExemplo; i++)
  {
    int n = numeros[i];
    Serial.println(n);
  }

  if(doc["hobbies"].is<JsonArray>()) //* Verifica se existe
  {
    //* JsonVariantConst -----> representa um valor Json qualquer: texto, numero, booleano, null, objeto ou array
    //* O tipo so eh conhecido em tempo de execucao, por isso eh necessario verificar com is<T>() e converter
    //* com as<T>. rh usado quando ainda nao se sabe ou nao importa o que esta sendo guardado ali
    //* 
    //* JsonArrayConst  representa um array Json, e oferece operacoes de array:
    //* array.size()    retorna quantidade de itens
    //* array.isNull()  retorna true se nao aponta para um array
    //* array.nesting() retorna a profundidade de alinhamento dentro do array
    //* array[i] = item na posicao i ex.: array[1].as<const char*>() retornaria um "texto"
    //* array[i] | deafult retorna intem ou valor padrao se null
    //* array[i].is<T>() retorna true para o teste de tipo T
    //* Acessar um indice fora do limite do array nao trava, retorna valor nulo (0, false, nullptr)
    //* 
    //* Em casos de array numero ou booleanos eh possivel copiar um array para outro (nao funciona com strings)
    //* int notas[10]; 
    //* size_t quantidadeItensCopiados = copyArray(doc["numeros"].as<JsonArrayConst()>, notas); 
    //* quando o vetor destino enche nao trava e devolve quando itens forem copiados 
    //* 

    for(JsonVariantConst v : doc["hobbies"].as<JsonArrayConst>()) //* Altera sobre os elementos do array
    {
      
      if(quantidade >= tamanhoVetor) break; //* Limita a quantidade de itens dentro de um vetor
      hobbies[quantidade++] = v | "Nao informado";

      for(size_t i = 0; i < quantidade; i++)
      {
        Serial.printf("Hobby %d: %S\n\r", i + 1, hobbies[i]);
      }

    }
  }

  Serial.printf("\n%stenho %s anos, nasci em %s, sou %s e torco para o %s",
                   nome.c_str(),
                   idade > 0 ? String(idade).c_str() : "Nao Informado", 
                   naturalidade.c_str(),
                   estadoCivil ? "solteiro" : "comprometido",
                   timeFutebol.c_str());

  //* ALINHAMENTO
  //* JEITO 01
  const char* rua = doc["endereco"]["rua"].as<const char*>();
  //* o "|" (pipe) ja esta fazendo a verificacao de tipo
  float lat = doc["endereco"]["coordenadas"]["lat"] | 0.0f;

  //* JEITO 02
  JsonObjectConst endereco = doc["endereco"];
  Serial.println(endereco["rua"].as<const char*>());
  Serial.println(endereco["coordenadas"]["lat"].as<float>());

  JsonObjectConst coordenadas = doc["endereco"]["coordenadas"];
  Serial.println(coordenadas["lat"].as<float>());
  Serial.println(coordenadas["lon"] | 0.0f);
}

void loop() 
{
  
}
//! Ataque DDoS ---> Ataque de sobrecarga, enviar varios pedidos por IPs diferentes
