# ProcessFlow

Orquestrador de processos em C que cadastra tarefas e as executa por meio de processos filhos, com suporte a execução sequencial, paralela e redirecionamento de entrada/saída.

## Sistema Operacional

Desenvolvido e testado em **Linux Ubuntu** (utiliza `fork()`, `execvp()`, `wait()`/`waitpid()` e `dup2()`)

## Arquivos do projeto

| Arquivo | Descrição |
|---|---|
| `main.c` | Loop do modo interativo (`processflow>`), faz a tokenização de cada linha digitada e despacha para as funções de tratamento de cada comando. |
| `tasks.c` | Implementa o cadastro de tarefas (`cadastrarTask`), busca de tarefas por nome (`buscaTask`), configuração de redirecionamento de entrada/saída (`input`, `output`, `append`, `redirecionar`) e execução de tarefas via `fork`/`execvp` (`runTask`, `runSequential`, `runParallel`). |
| `tasks.h` | Definição da struct `Tarefa` e das assinaturas das funções usadas em `main.c` e `tasks.c`. |

## Compilação

```bash
make
```

## Como executar

Modo interativo (sem argumentos):

```bash
./processflow
```

O programa exibirá o prompt `processflow>` aguardando comandos.

*Modo workflow (arquivo `.pf` como argumento) — **ainda não implementado**

## Comandos suportados (modo interativo)

```
task <nome> <programa> [argumentos...]   # cadastra uma tarefa
run <nome>                               # executa uma tarefa cadastrada
run sequential <t1> <t2> ...             # executa tarefas em sequência
run parallel <t1> <t2> ...               # executa tarefas em paralelo
input <nome> <arquivo>                   # redireciona entrada da tarefa a partir de um arquivo
output <nome> <arquivo>                  # redireciona saída da tarefa para um arquivo (sobrescreve)
append <nome> <arquivo>                  # redireciona saída da tarefa para um arquivo (acrescenta)
exit                                     # encerra o ProcessFlow
```

### Exemplo de uso

```
processflow> task listar /bin/ls -l
Tarefa listar - /bin/ls cadastrada com sucesso
processflow> output listar saida.txt
processflow> run listar
processflow> exit
```
