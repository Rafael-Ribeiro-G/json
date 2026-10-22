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
    "hobbies" : ["academia", "jogos", "esportes"]
  }
    )";

  JsonDocument doc;
  DeserializationError erroJson = deserializeJson(doc, resposta);
  //! Clausula de guarda, imprime caso JSOn tenha algum erro
  if(erroJson)
  {
    Serial.printf("Erro: %s\n\r", erroJson.c_str());
    return;
  }
  
  //* Verifica se existe a chave e se o tipo é adequado
  String nome = "";
  if(doc["nome"].is<const char*>())
  nome = "Meu nome é " + doc["nome"].as<String>() + ", ";
  
  //* Verifica a existencia da chave, caso nao existir passasse um valor negativo
  //* para indicar a inexistencia, pois 0 neste caso eh um valor possivel
  int idade = doc["idade"] | -1;
  
  //* Verificar a existencia da chave e um valor padrao caso nao existir
  String naturalidade = doc["naturalidade"] | "Nao informado";
  
  //*Normalmente uma variavel booleana tem como valor padrao tem como valor padrao false
  bool estadoCivil = doc["isSingle"] | false;
  
  String timeFutebol = doc ["time"];
  
  int temperatura;
  temperatura = 36;
  if(doc["temperatura"].is<int>()) //* a variavel so recebera um novo valor se a chave existir
  temperatura = doc["temperatura"];

  //* =============================== TRATAMENTO DE VETORES ===============================
  const int tamanho = 10;
  const char* hobbies[10];
  size_t quantidade = 0;
  //* sizeof retorna o tamanho do vetor em bytes, porém cada posicao pode ter mais que um byte,
  //* por isso dividimos o tamanho total pelo tamanho de um item do vetor existente, 
  //* como sempre existe a posicao [0], utilizamos ela como padrao
  int tamanhoVetor = sizeof(hobbies) / sizeof(hobbies[0]);
  if(doc["hobbies"].is<JsonArray>()) //* Verifica se existe
  {
    JsonArrayConst qntIndicesArray = doc["hobbies"].as<JsonArrayConst>();

    for(JsonVariantConst v : doc["hobbies"].as<JsonArrayConst>()) //* foreach nao existe em c mas isso eh semelhante
    {
      
      if(quantidade >= tamanhoVetor) break; //* Limitando a quantidade de itens dentro de um vetor
      hobbies[quantidade++] = v | "";
    }
  }

  for(int i = 0; i < 3; i++)
  {
    Serial.println(hobbies[i]);
  }

  Serial.printf("\n%stenho %s anos, nasci em %s, sou %s e torco para o %s",
    nome.c_str(),
    idade > 0 ? String(idade).c_str() : "Nao Informado", 
    naturalidade.c_str(),
                   estadoCivil ? "solteiro" : "casado",
                   timeFutebol.c_str());

}
void loop() 
{
  
}
//! Ataque DDoS ---> Ataque de sobrecarga, enviar varios pedidos por IPs diferentes
