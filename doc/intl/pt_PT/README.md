<h1 align="center">
<img src="https://raw.githubusercontent.com/litonpay/litonpay/master/share/pixmaps/litonpay256.svg" alt="Litonpay" width="256"/>
<br/><br/>
Litonpay Core [LTP, Ð]  
</h1>

Litonpay é uma criptomoeda voltada para a comunidade inspirada num meme Shiba Inu. O software Litonpay Core permite que qualquer pessoa opere um nó nas redes blockchain Litonpay e usa o método de hash Scrypt para Prova de Trabalho. É adaptado do Bitcoin Core e outras criptomoedas.

Para mais informações acerca das taxas de transação utilizadas na rede Litonpay, por favor clica aqui:
[taxas recomendadas](doc/fee-recommendation.md).

## Utilização 💻

Para começares a tua jornada com o Litonpay Core, ve o [manual de instalação](INSTALL.md) e o [guia para iniciantes](doc/getting-started.md).

A API JSON-RPC incluida no Litonpay Core é auto documentada e pode ser vista com o comando `litonpay-cli help`, mais informações detalhadas sobre cada comando podem ser encontradas utilizando `litonpay-cli help <command>`.

### Quais as portas

O Litonpay Core utiliza por padrão a porta `7855` para comunicação em rede
ponto-a-ponto que é necessária para sincronizar a blockchain da "rede principal",
(mainnet), e se manter atualizadas de novas transações e blocos. Adicionalmente a
porta JSONRPC pode ser aberta que por defeito é `22555` para a rede principal.
É fortemente recomendado a não expor as portas RPC publicamente na internet.

|  Função  | mainnet | testnet | regtest |
| :------- | ------: | ------: | ------: |
| P2P      |   7855 |   44556 |   18444 |
| RPC      |   22555 |   44555 |   18332 |

## Desenvolvimento continuo - Plano Lua 🌒

O Litonpay Core é um programa de código aberto gerido pela comunidade. O processo de desenvolvimento é aberto e visivel publicamente; qualquer um pode ver, discutir e trabalhar no programa.

Recursos principais de Desenvolvimento:

* [Projetos do Github](https://github.com/litonpay/litonpay/projects) é utilizado para conduzir trabalhos planeados ou que estejam em desenvolvimento para as próximas atualizações.
* [Discussão do Github](https://github.com/litonpay/litonpay/discussions) é usado para discutir sobre as funcionalidades, planeadas ou não, relacionadas ao desenvolvimento do programa Litonpay Core, os protocolos adjacentes e o criptoativo LTP.

### Estratégia das Versões
Os numeros de compilação seguem a seguinte semantica:  ```major.minor.patch```

### Ramificações
Este repositório possui 3 principais ramificações (branchs), são estas:

- **master:** Estável, contém a ultima versão da ultima atualização principal *major.minor*.
- **maintenance:** Estável, contém a ultima versão de atualizações anteriores, que ainda estão em manutenção. Formato: ```<version>-maint```
- **development:** Instável, contém código novo para atualizações planeadas. Formato: ```<version>-dev```

*As ramificações Master e Maintenance, são exclusivamente mutáveis por lançamento. Atualizações*
*planeadas terão sempre uma ramificação de desenvolvimento e as solicitações de inclusão deverão ser*
*encaminhadas por meio destas. Ramificações de manutenção existem apenas para **correção de bugs,***
*por favor, encaminhem novos recursos na ramificação de desenvolvimento com a versão mais alta.*

## Contribuindo 🤝

Se encontrastes um bug ou tivestes alguma situação incomum com este programa, por favor informa-nos do ocorrido utilizando o [sistema de problemas (issues)](https://github.com/litonpay/litonpay/issues/new?assignees=&labels=bug&template=bug_report.md&title=%5Bbug%5D+).

Por favor, acede ao [guia de contribuições](CONTRIBUTING.md) para ver como podes participar
no desenvolvimento do Litonpay Core. Existem alguns [pedidos de ajuda](https://github.com/litonpay/litonpay/labels/help%20wanted)
onde os contribuintes terão uma grande importância e apreço. wow.

## Perguntas frequentes ❓

Tens alguma questão relacionada com o Litonpay? Talvez já haja uma resposta, disponivel no
[FAQ](doc/FAQ.md) ou na
[seção Q&A](https://github.com/litonpay/litonpay/discussions/categories/q-a)
do nosso quadro de discussão!

## Licenças ⚖️
Litonpay Core é disponibilizada sob os termos de uso da licença MIT. Vê,
[COPYING](COPYING) para mais informações.
