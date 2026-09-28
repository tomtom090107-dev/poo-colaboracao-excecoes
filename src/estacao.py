from abc import ABC, abstractmethod


class SensorNivel:
    def __init__(self, valor):
        self._valor = valor

    def valor(self):
        return self._valor

    def atualizar(self, valor):
        self._valor = valor


class PainelFixo:
    def __init__(self, sensor):
        self._sensor = sensor

    def leitura(self):
        return self._sensor.valor()


class IFonteLeitura(ABC):
    @abstractmethod
    def valor(self):
        raise NotImplementedError

    @abstractmethod
    def unidade(self):
        raise NotImplementedError


class FonteNivel(IFonteLeitura):
    def __init__(self, sensor):
        self._sensor = sensor

    def valor(self):
        return self._sensor.valor()

    def unidade(self):
        return "%"


class FonteConstante(IFonteLeitura):
    def valor(self):
        return 42.5

    def unidade(self):
        return "%"


class FalhaLeitura(Exception):
    pass


class FalhaCalibracao(FalhaLeitura):
    pass


class Sessao:
    def __init__(self):
        self.abertas = 0

    def abrir(self):
        self.abertas += 1

    def fechar(self):
        self.abertas -= 1


def adquirir(fonte, disponivel, calibrada, sessao):
    sessao.abrir()
    try:
        if not disponivel:
            raise FalhaLeitura("fonte indisponivel")
        if not calibrada:
            raise FalhaCalibracao("fonte sem calibracao")
        return fonte.valor()
    finally:
        sessao.fechar()


def ler_servico(fonte, disponivel, calibrada, sessao):
    return adquirir(fonte, disponivel, calibrada, sessao)


def executar_ciclo(fonte, disponivel, calibrada, sessao):
    try:
        return True, ler_servico(fonte, disponivel, calibrada, sessao), ""
    except FalhaCalibracao:
        return False, 0, "calibracao"
    except FalhaLeitura:
        return False, 0, "indisponivel"