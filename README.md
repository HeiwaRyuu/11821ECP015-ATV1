Primeira atividade SEMB1 UFU - 11821ECP015

Primeiramente intalei Ubuntu 20.04 para utilizar no WSL2 através do Windows PowerShell, 
configurei meu usuário e senha e atualizei o sistema operacional com "sudo apt update"
e "sudo apt upgrade". Em seguida criei os diretórios Downloads e semb1-workspace, instalei
o git e o configurei com nome e email. Clonei o conteúdo do repositório do professor Daniel
para meu diretório semb1-workspace, o qual continha os arquivos de "rules" ou regras da stlink.

Realizei a instalação da GCC ARM Toolchain, e das ferramentas de gravação e depuração de código
(OpenOCD, ST-LINK Tools, Visual Studio Code (VS Code)). A instalação do VS Code foi realizada
tanto no Linux quanto no Windows, em conjunto com suas configurações e downloads das principais
extensões: C/C++, C/C++ Extension Pack, C/C++ Themes e Cortex-Debug.

Em seguida, instalei o USBIP no Linux, porém antes de conseguir realizar esta etapa de maneira
correta, foi necessária atualizar o kernel do sistema operacional, como a versão estava desatualizada
a requisição de instalação estava gerando erro de incompatibilidade. Durante a instação dos pacotes
linux-tool-generic e hwdata, ocorreu um erro de fetch do link para download, porém o problema foi
resolvido através da atualização do "apt-get update".

Configurei o ambiente do Linux com os pacotes necessários para a conexão do ST-LINK e suas alterações
e configurações para ajustar os direitos de acesso ao aparelho, e através do Windows PowerShell, 
realizei o "attach" do aparelho ao Linux utilizando o "usb ip" da porta USB na qual o inseri no computador
e finalmente, através do command line do linux, confirmei se o sistema operacional estava reconhecendo o
ST-LINK de maneira correta.
