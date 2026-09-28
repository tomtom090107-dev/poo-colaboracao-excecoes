#pragma once

#include <stdexcept>

class SensorNivel {
    double valor_;
public:
    explicit SensorNivel(double valor) : valor_(valor) {}
    double valor() const { return valor_; }
    void atualizar(double valor) { valor_ = valor; }
};

// Associacao sem posse: o sensor deve viver mais que o painel.
class PainelFixo {
    const SensorNivel* sensor_;
public:
    explicit PainelFixo(const SensorNivel& sensor) : sensor_(&sensor) {}
    double leitura() const {
        return sensor_->valor();
    }
};

class IFonteLeitura {
public:
    virtual ~IFonteLeitura() = default;
    virtual double valor() const = 0;
    virtual const char* unidade() const = 0;
};

class FonteNivel final : public IFonteLeitura {
    const SensorNivel& sensor_;
public:
    explicit FonteNivel(const SensorNivel& sensor) : sensor_(sensor) {}
    double valor() const override { return sensor_.valor(); }
    const char* unidade() const override { return "%"; }
};

class FonteConstante final : public IFonteLeitura {
public:
    double valor() const override { return 42.5; }
    const char* unidade() const override { return "%"; }
};

class FalhaLeitura : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class FalhaCalibracao : public FalhaLeitura {
public:
    using FalhaLeitura::FalhaLeitura;
};

class Sessao {
    int& abertas_;
public:
    explicit Sessao(int& abertas) : abertas_(abertas) { ++abertas_; }
    ~Sessao() { --abertas_; }
    Sessao(const Sessao&) = delete;
    Sessao& operator=(const Sessao&) = delete;
};

inline double adquirir(const IFonteLeitura& fonte, bool disponivel,
                      bool calibrada, int& abertas) {
    Sessao sessao{abertas};
    if (!disponivel) throw FalhaLeitura("fonte indisponivel");
    if (!calibrada) throw FalhaCalibracao("fonte sem calibracao");
    return fonte.valor();
}

inline double lerServico(const IFonteLeitura& fonte, bool disponivel,
                        bool calibrada, int& abertas) {
    return adquirir(fonte, disponivel, calibrada, abertas);
}

struct ResultadoLeitura {
    bool sucesso;
    double valor;
    const char* motivo;
};

inline ResultadoLeitura executarCiclo(const IFonteLeitura& fonte, bool disponivel,
                                     bool calibrada, int& abertas) {
    try {
        return {true, lerServico(fonte, disponivel, calibrada, abertas), ""};
    } catch (const FalhaCalibracao&) {
        return {false, 0, "calibracao"};
    } catch (const FalhaLeitura&) {
        return {false, 0, "indisponivel"};
    }
}