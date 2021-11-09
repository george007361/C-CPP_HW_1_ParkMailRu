echo "Stress testing... ${0}"

# Парсим аргументы
start_data_size=${1}
end_data_size=${2}
step_data_size=${3}
shift 3
format_str=$*
echo "Min data size: $start_data_size Mb; Max data size $end_data_size Mb; Step: $step_data_size; Scrtipt args: $format_str"
# Удалить прошлые тесты
if [ -d "./test_log" ];
then
    echo "Removing test_log/"
    rm -r test_log
fi
mkdir test_log

# Количество тестов
for current_data_size in $(seq $start_data_size $end_data_size)
do
    echo $current_data_size
    # Папка под логи
    cd ./test_log
    mkdir log_data_size_${current_data_size}_Mb
    curr_log_path=`pwd`/log_data_size_${current_data_size}_Mb
    cd ../
    
    # Тесты
    cd ../test_on_rand_data/
    sh test_on_rand_data.sh $format_str
    cd ./test_log
    mv * $curr_log_path
done