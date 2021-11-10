echo "Stress testing... ${0}"
shift 1
# Парсим аргументы
tests_count=${1}
test_data_len=${2}
bufsize=${3}
format_str=" --bufsize $bufsize --filepath ../tmp_data/data.txt"
shift 3
while [ -n "$1" ]
do
    echo $1
    format_str=$format_str" --graph \""$1"\""
    shift
done
echo "Tests count: $tests_count; Test data size $test_data_len Mb; Buffer size: $bufsize; Function args: $format_str"

# Удалить данные с прошлых тестов
if [ -d "./test_log" ];
then
    echo "Removing test_log/"
    rm -r test_log
fi
mkdir test_log

if [ -d "./tmp_data" ];
then
    echo "Removing tmp_data/"
    rm -r tmp_data
fi
mkdir tmp_data

if [ -d "./tmp_build" ];
then
    echo "Removing tmp_build/"
    rm -r tmp_build
fi
mkdir tmp_build

# Создать тестовые данные данные
cd ./tmp_data
tr -dc 'A-Za-z,!().:' < /dev/urandom | head -c ${test_data_len}M > data.txt
cd ../

# Запускаем заданное число проверок
for i in `seq 1 $tests_count`
do
    # Тесты
    echo "----------$i---------------">>./test_log/log-building-tests.txt
    cd ./tmp_build
    echo "------Testing serial $i------">>../test_log/${i}_iterarion.txt
    cmake -DUSE_SERIAL=0 ../../../ >>../test_log/log-building-tests.txt
    make >>../test_log/log-building-tests.txt
    ./main $format_str >>../test_log/${i}_iterarion.txt
    
    echo "------Testing parallel $i------">>../test_log/${i}_iterarion.txt
    cmake -DUSE_SERIAL=0 ../../../ >>../test_log/log-building-tests.txt
    make >>../test_log/log-building-tests.txt
    ./main $format_str >>../test_log/${i}_iterarion.txt
    cd ../
done

# Удалить временные файлы
if [ -d "./tmp_build" ];
then
    rm -r tmp_build
fi

if [ -d "./tmp_data" ];
then
    rm -r tmp_data
fi
