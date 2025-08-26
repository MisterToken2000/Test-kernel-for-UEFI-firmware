def hex_string_to_byte_array(hex_str):
    # Удаляем пробелы
    hex_str = hex_str.replace(" ", "")

    # Проверка чётности длины
    if len(hex_str) % 2 != 0:
        raise ValueError("Длина hex-строки должна быть чётной")

    # Разбиваем по 2 символа (один байт), добавляем 0x и разделяем запятыми
    bytes_list = [f"0x{hex_str[i:i+2]}" for i in range(0, len(hex_str), 2)]

    return ", ".join(bytes_list)


# Пример использования
input_str = "aaaa 0001 8000 0001 8000 4a51 ea50 5a51 c99e 0001 8000 0001 8000 0001 8000 5555"
result = hex_string_to_byte_array(input_str)
print(result)
