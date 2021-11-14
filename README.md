# strezdout
A budget stress tester for your C++ projects.  
[По-русски](#я-русский)

## What
A tool for stress testing your lab solution with randomly generated tests. Features include:
- Prints execution time of your program for each test
- Prints correctness of your answer (checked using a pre-compiled binary with someone's correct solution)
- Average execution time for each category of tests

In each of the printed categories "init time" represents the time needed to construct your solution (constructors of classes, file reading, etc).

## How
1. Install Docker
2. Clone this repository
3. Simply run
```bash
./test.py {lab name} /path/to/your/src/folder /path/to/your/include/folder```
```
Passing the `-print` flag will print each test so that you can debug your solution.  
{lab name} can be one of `wordnet`, `2d-tree` or `8-puzzle`

### Note
When updating (`git pull`) this repo, make sure to run everything with the `-rebuild` flag, so that the docker container is rebuilt.

## Inner workings
A generator and another person's correct solution are built using a specific Docker container. All symbols and metainformation are stripped from the built binaries (generator executable + solution shared library) in order to complicate copying other people's work. When you run `test.py`, a packaged jar-file with all of the clojure code is launched inside a Docker container with the same env as during build time, which then compiles your solution and launches 3 processes in parallel - the generator, validator and your solution. The generator simply spurts out tests which are lazily read (to save memory) and piped into your program and the validator, outputs of which are then checked for equality. 


# Я русский
Бюджетный стресс тестер ваших проектов на плюсах

## Что
Протестирует ваш репозиторий с задачей с помощью подготовленных тестов из генераторов, подготовленных другими людьми.  
Выведет время исполнения каждого теста, корректность вашего ответа, среднее время исполнения по категории.  
В каждой категории время init time обозначает то время, которое понадобилось на изначальную инициализацию классов (если такая есть, например прочитать какие-то файлы)

## Как
1. По классике устанавливаем docker
2. Клонируем репу 
3. ```./test.py {задача} /путь/к/вашей/папке/src /путь/к/вашей/папке/include```  
Также можно передать флаг `-print`, который выведет каждый тест  
{задача} может быть одно из: `wordnet`, `2d-tree`, `8-puzzle`

### Осторожно!
При обновлении репозитория нужно запускать с флагом `-rebuild` чтобы докер-контейнер сбилдился заново.
 
