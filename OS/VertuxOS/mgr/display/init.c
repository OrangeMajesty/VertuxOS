/*
	Дисплей Менеджер.
	* Построение очереди явного доступа к видео драйверу.
	*
	Загововок header.c
	ver 0.1
*/
void starting() {
	char *a = (void*)0xb8000;
	for (int i = 0; i < 0x200; ++i)
	{
		a[i*2] = '!';
	}
	// Проверка загружен ли драйвер и нет ли ошибок.
	// if(Driver_flags) {
	// 	load_driver(configure.file);
	// }
}
void loop() {

}
void finite() {

}