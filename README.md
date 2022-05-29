Primeira atividade SEMB1 UFU - 11821ECP015

Primeiramente intalei Ubuntu 20.04 para utilizar no WSL2 através do Windows PowerShell, 
configurei meu usuário e senha e atualizei o sistema operacional com "sudo apt update"
e "sudo apt upgrade". Em seguida criei os diretórios Downloads e semb1-workspace, instalei
o git e o configurei com nome e email. Clonei o conteúdo do repositório lab-01 do professor Daniel
para meu diretório semb1-workspace, o qual continha os arquivos de "rules" ou regras da stlink
que foram utilizados posteriormente para alterar as permissões de acesso do linux à porta usb
a qual o ST-LINK foi inserido.

Realizei a instalação da GCC ARM Toolchain, e das ferramentas de gravação e depuração de código
(OpenOCD, ST-LINK Tools, Visual Studio Code (VS Code)). A instalação do VS Code foi realizada
tanto no Linux quanto no Windows, em conjunto com suas configurações e downloads das principais
extensões: C/C++, C/C++ Extension Pack, C/C++ Themes e Cortex-Debug.

Em seguida, instalei o USBIP no Linux, porém antes de conseguir realizar esta etapa de maneira
correta, foi necessária atualizar o kernel do sistema operacional. Como a versão estava desatualizada
a requisição de instalação estava gerando erro de incompatibilidade. E durante a instação dos pacotes
linux-tool-generic e hwdata, ocorreu um erro de fetch do link para download, porém o problema foi
resolvido através da atualização do "apt-get update".

Configurei o ambiente do Linux com os pacotes necessários para a conexão do ST-LINK e suas alterações
e configurações para ajustar os direitos de acesso ao aparelho, e através do Windows PowerShell, 
realizei o "attach" do aparelho ao Linux utilizando o "usb ip" da porta USB na qual o inseri no computador
e finalmente, através do command line do linux, confirmei se o sistema operacional estava reconhecendo o
ST-LINK de maneira correta.

Com essas configurações iniciais prontas, iniciei a criação dos códigos. O foco dessa primeira etapa foi
o processo de compilação e geração dos arquivos objetos realocáveis (.o). Foi criado um arquivo "main.c"
com algumas linhas de código que a princípio nao fazem nada, com o intuito de entender as tags de compilação
essenciais, como a indicação de qual o tipo de processador (-mcpu), qual o conjunto de instruções (neste caso
o conjunto de instruções reduzido de 16-bits, indicado com a tag -mthumb), e a tag principal que indica para que
seja realizada somente a etapa de compilação (-c). Após a criação do main.c, executei a instrução de compilação
diretamente na linha de comando, para testar se as configurações anteriores estavam corretas, e tudo fluiu normalmente.

Posteriormente, iniciei o desenvolvimento do arquivo Makefile, que é um arquivo que contém regras para execução de linhas
de comando. Além de evitar com que haja a necessidade de ficar reescrevendo a linha de comando para a compilação de um código,
também evita erros de sintaxe e facilita a compilação de diversos arquivos caso seu programa contenha muitas dependências distintas.

Comecei a construção do código com a introdução de uma regra, que indica qual é o alvo (arquivo gerado) e qual são suas dependências 
(os arquivos necessários para que o alvo seja gerado), e em seguida uma receita, que indica como esse alvo deve ser gerado. Além da criação
das regras de compilação, criei uma regra para executar um comando que limpava os arquivos objetos gerados, para que toda vez que formos realizar
a compilação, todo o processo seja realizado do zero, gerando todos os arquivos novamente, evitando conflito de versões.

Introduzi o uso de variáveis na criação do código, para aos poucos criar um arquivo generalizado, fácil de ser alterado
caso seja necessário a introdução de diversos novos arquivos para serem compilados, ou seja necessário alterar as tags
de compilação.

Após gerar um protótipo inicial do arquivo Makefile, com algumas regras de compilação, também é necessário a criação de alguns arquivos
que são previamente executados ao main(), um deles é o startup ou arquivo de inicialização. Aqui definimos a inicialização dos stacks,
transferimos dados de certas seções para a RAM, e diversas outras atividades extremamente necessárias para o funcionamento do código no aparelho.

Dentro do startup.c, inicializei o stack, declarei e inicializei a tabela de vetores de interrupção (isr vectors), o código da função Reset handler,
em conjunto com os outros códigos para funções de excessão, que são chamadas quando há uma interrupção. 

Para a inicialização do stack, defini alguns valores baseados na arquitetura que estamos utilizando (cortex-m). Como o stack é uma pilha que armazena informações 
de maneira temporária durante a execução de uma função, quando esta função é finalizada a stack ou pilha é esvaziada. Como são dados temporários que serão armazenados, uma maneira
interessante de armazenar esses dados é utilizando a memória SRAM. Para isso temos de saber aonde a SRAM começa e termina, com essas informações em mãos, podemos gerar o stack
e apontar o nosso stack pointer para o endereço desejado.

Dentro do codigo defini esses dados e apontei o stack pointer para o início da Flash, já que a arquitetura cortex-m consegue carregar o endereço do stack pointer direto da flash.
Para que seja realizado o processo de partida, outro pré-requisito essencial além do stack pointer no endereço inicial, é o primeiro vetor de interrupção reset handler, logo no
próximo endereço (4 bytes após o endereço inicial).

Porém, para evitar que uma excessão não tratada acabe gerando um comportamento inesperado ou indesejado, defini toda a tabela de vetores de interrupção, os 15 iniciais reservados pelo
processador, e os 86 restantes logo em seguida. E para isso, no início da flash reservei espaço suficiente para todos os vetores de interrução que o aparelho pode gerar, no caso, cada
vetor ocupa 4 bytes, como existem 102 vetores, reservamos 408 bytes.

É também necessário garantir que a tabela de vetores seja alocada no início da flash, como a alocação é realizada durante o processo de linkedição, precisamos informar ao linker que esta
seção do código deve ser armazenada em um local específico na flash. Para que isso seja realizado, adicionei a seção (.isr_vectors) como um atributo ao vetor de interrupções. Também criei
uma nova rotina definida como reset_handler, que será responsável por copiar o conteúdo da seção data da flash para a SRAM, e inicializar a seção .bss em zero. E para as demais excessões
criei uma função genérica chamada default_hanlder, e a coloquei como alias para todas as demais excessões, com a tributo "weak", para caso eu crie uma função sem o atributo weak, ela sobrescreva
default_handler e seja usada no lugar dela.

Em seguida, defini as ações que a função reset_handler deve realizar: copiar os dados da seção .data para a SRAM, preencher com zeros a seção .bss e por fim, chamar a função main().
Para realizar essas tarefas, é necessário conhecer o início e o fim das seções .data e .bss. Neste arquivo elas serão definidas como variáveis externas, já que no processo de linkedição
essas informações estão disponíveis. Copiar os dados da seção .data para a memória SRAM é bastante simples, basta iterarmos por toda a seção na memória FLASH e copiar os dados para o endereço 
de destino na memória SRAM. Da mesma forma, para escrever zero na seçao .bss iremos iterar por toda a seção na SRAM escrevendo zero em cada posição. Ao final deste processo basta chamar a função main().

Após a criação destas etapas no arquivo startup.c, adicionei ele como um arquivo que deve ser compilado ao arquivo Makefile, para que seja gerado o arquivo "startup.o".

Após a finalização do arquivo startup.c, criei o programa para piscar o LED e escrevi o linker script. Para desenvolver o programa para piscar o LED deve-se determinar em qual pino o LED está conectado e que qual
o valor que deve-se atribuir a este pino para ligar/desligar o LED. É possível ver que na esquemática, o pino conectado ao LED é o 13 da porta C, e que ao colocar este pino em nível 0, liga-se o LED. Também é necessário
ligar o clock da porta C, já que por padrão ele vem desligado para economizar energia.
Para ligar o clock da porta C, fiz uso do periférico Reset and Clock Control (RCC), que neste caso é ligado ao barramento AHB1. Para ligar o clock do nosso GPIOC, ajusta-se o bit 2 do registrador RCC_AHB1ENR para 1.
Defini o endereço base de cada um dos periféricos que serão utilizados, justamente para calcularmos seu devido offset e alterar somente os endereços de memória que desejamos. Alterei o pino PC13 para trabalhar como um
pino de saída, para que o LED possa ser acionado, com saída "push-pull", sem capacidade de pull-up ou pull-down.

Depois de configurar o pino da maneira correta para que o LED possa ser recrutado, alterei o estado do pino de saída através do port bit set/reset register, que permite alterar somente o pino desejado, ao invés de ter
que alterar o estado de todos os outros pinos, e depois reescrever no registrador somente o pino alterado, que daria muito mais trabalho. Além disso, criei uma variável global que indica o status do led, ou seja, altera
seu estado de ligado para desligado, e vice e versa. Adicionei também um vetor estático constante que representa a versão do programa, V1.0, já que essa variável não deve ser alterada de maneira alguma.

Em seguida, desenvolvi o arquivo linker script, que resolverá a parte de combinar os arquivos objetos em um único arquivo executável. Aqui todas as seções dos arquivos objetos de entrada são mapeadas e é aonde controla-se
o layout da memória no arquivo de saída. Defini o ponto de entrada do nosso linker script como sendo a nossa função reset_handler, defino as seções de memória e suas informações, como as permissões de escrita/leitura, endereço
de início, e comprimento. Em seguida, criei as diferentes seções de saída do arquivo, e suas características. É aqui que intruí que o vetor de interrupções armazenado na seção .isr_vector deve ser armazenado na flash em conjunto
com outras seções. Defini também que a seção data deve ser gravada na memória SRAM e também na flash, para evitar que seus valores sejam alterador durante a compilação e criação do executável final. E armazenei a seção .bss
somente na SRAM. O último passo foi exportar as variáveis utilizadas como variáveis externas no startup.c, e aqui temos acesso, sendo elas os endereços de início e fim das seções .data e .bss e forçar que o acesso a memória seja
realizado de 4 em 4 bytes para alinhar a memória corretamente através da função "ALIGN(4)", aonde o número 4 representa o número de bytes que deve ser alinhado em cada operação.

Finalmente, após a criação do linker script, modifiquei pela última vez o arquivo Makefile, adicionando novas variáveis, para armazenar o nome do linker e as opções de linkedição.





