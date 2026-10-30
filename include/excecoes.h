#ifndef EXCECOES_H
#define EXCECOES_H

#include <stdexcept>
#include <string>

// Hierarquia de exceções customizadas do jogo.
// Todas herdam de JogoException, que herda de std::runtime_error,
// então um único catch (const JogoException&) captura qualquer erro do jogo.

class JogoException : public std::runtime_error {
public:
    explicit JogoException(const std::string& msg) : std::runtime_error(msg) {}
};

// Lançada quando um arquivo de mapa não existe ou está vazio
class ArquivoMapaException : public JogoException {
public:
    explicit ArquivoMapaException(const std::string& arquivo)
        : JogoException("Nao foi possivel carregar o mapa: " + arquivo) {}
};

// Lançada ao pedir um mapa ou objeto que não existe
class MapaInvalidoException : public JogoException {
public:
    explicit MapaInvalidoException(const std::string& msg)
        : JogoException("Mapa invalido: " + msg) {}
};

// Lançada ao tentar comprar algo sem dinheiro suficiente
class SaldoInsuficienteException : public JogoException {
public:
    SaldoInsuficienteException(float preco, float carteira)
        : JogoException("Saldo insuficiente: custa $" + std::to_string((int)preco) +
                        " e voce tem $" + std::to_string((int)carteira)) {}
};

// Lançada ao fisgar um peixe com o estoque já cheio
class EstoqueCheioException : public JogoException {
public:
    explicit EstoqueCheioException(unsigned int capacidade)
        : JogoException("Estoque cheio (" + std::to_string(capacidade) +
                        " peixes)! Va ao mercado vender antes de pescar mais.") {}
};

// Lançada ao tentar ir pescar sem energia suficiente
class SemEnergiaException : public JogoException {
public:
    SemEnergiaException(unsigned int necessaria, unsigned int atual)
        : JogoException("Sem energia: pescar gasta " + std::to_string(necessaria) +
                        " e voce tem " + std::to_string(atual) + ".") {}
};

#endif
