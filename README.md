# Soal Shift Modul 2 Sisop 2021 (Kelompok F12)
#### Nama anggota kelompok:
- Farhan Arifandi (05111940000061)
- Fitrah Mutiara (05111940000126)
- M. Iqbal Abdi (05111940000151)

## Soal 1

## Soal 2
### Poin (a)
Mengunzip file yg bernama pets.zip ke direktori yg sudah di declare di atas, yaitu /home/user/modul2/petshop
- -p pada mkdir digunakan untuk untuk membatalkan pembuatan folder bila nama folder sudah ada
- chmod digunakan untuk mengatur hak akses atau permission terhadap suatu file/direktori kepada user
- `(strcmp(direntPtr->d_name, ".") != 0) && (strcmp(direntPtr->d_name, "..") != 0))` untuk menghindari file . dan juga ..
- rm pada `/bin/rm` digunakan untuk menghapus folder yang tidak perlu
```c
pid_t child_id = fork();
if (child_id == 0) execl("/bin/mkdir", "/bin/mkdir", "-p", directory, NULL);
else (wait(&status) > 0);
	
child_id = fork();
if (child_id == 0) execl("/bin/unzip", "/bin/unzip", "pets.zip", "-d", directory, NULL);
else (wait(&status) > 0);
	
	//untuk mengatur hak akses atau permission terhadap suatu file/direktori kepada user
if(child_id == 0) execl("/bin/chmod", "/bin/chmod", "u+w", directory, NULL);
else (wait(&status) > 0);

while((direntPtr = readdir(dir)) != NULL){
if ((direntPtr->d_type == DT_DIR) && (strcmp(direntPtr->d_name, ".") != 0) && (strcmp(direntPtr->d_name, "..") != 0)){
    	child_id = fork();
		if(child_id < 0){
    		exit(EXIT_FAILURE);
		}
		if(child_id == 0){
		char pathName[300];
		sprintf(pathName, "%s/%s", directory, direntPtr->d_name);
                execl("/bin/rm", "/bin/rm", "-r", pathName, NULL);
		}
	}
}
```
### Poin (b)
Membuat folder untuk setiap jenis peliharaan yang ada dalam zip
- melakukan iterasi dari 0 hingga count 
- `mkdir` digunakan untuk membuat direktori dengan nama variabel folderName
```c
for(i = 0; i < count; i++){
	child_id = fork();
	if(child_id < 0){
	exit(EXIT_FAILURE);
	}
	if(child_id == 0){
	char folderName[200];
	sprintf(folderName, "%s/%s", directory, animals[i]);
        execl("/bin/mkdir", "/bin/mkdir", folderName, NULL);
	}
}
```
- Sebelumnya, masing-masing hewan dilakukan iterasi untuk diambil tiap nilai nama file dengan delimiter `;`
```c
while (fileName[i] != ';'){
	temp[i] = fileName[i];
	if(fileName[++i] == ';') break;
}
```
### Poin (c)
Pada poin ini, program akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan
- cp pada `/bin/cp` melakukan copy file 
```c
pid_t child_id = fork();		
if(child_id < 0)  exit(EXIT_FAILURE);
if(child_id == 0) execl("/bin/cp", "/bin/cp", img, newName, NULL);
while(wait(&status) > 0);
```
- Sebelum di copy pada sintaks di atas di dalam fungsi, melakukan copy dengan strcpy dari `direntPtr->d_name` ke fileName
- `direntPtr->d_name` merupakan nama-nama jenis hewan 
- `fileName[strlen(fileName)-4] = '\0'` digunakan untuk omit atau menghilangkan extensionnya
```c
char fileName[300], source[500];
//lanjutan poin c sebelum dicopy di fungsi diatas, direntptr d name itu jenis hewan 
sprintf(source, "%s/%s", directory, direntPtr->d_name);
strcpy(fileName, direntPtr->d_name);

fileName[strlen(fileName)-4] = '\0';
```
### Poin (d)
Pada poin  ini satu foto yang terdapat satu atau lebih dari peliharaan, maka foto harus di pindah ke masing-masing kategori folder yang sesuai
- `strtok(fileName, "_")` dan `strtok(NULL, "_")` digunakan untuk melakukan pemisahan nama file dengan delimiter `_` sehingga didapatkan nama hewan tiap individu
- rm pada `/bin/rm` berfungsi untuk menghapus folder yang tidak perlu
```c
char *firstAnimal = strtok(fileName, "_");
char *secondAnimal = strtok(NULL, "_");
				
if(secondAnimal) group_images(source, secondAnimal);
group_images(source, firstAnimal);
				
child_id = fork();
if(child_id < 0)  exit(EXIT_FAILURE);
if(child_id == 0) execl("/bin/rm", "/bin/rm", "-rf", source, NULL);
				
while(wait(&status) > 0);
```
### Poin (e)
Membuat sebuah file "keterangan.txt" yang berisi nama dan umur semua peliharaan dalam folder tersebut
-  strtok pada kasus ini digunakan untuk membagi string menjadi beberapa bagian yang dibatasi oleh karakter `;`
```c
char	*jenis = strtok(fileName, ";"), 
	*nama = strtok(NULL, ";"),
	*umur = strtok(NULL, ";"),
```
- sprintf() digunakan untuk dapat menulis string yang diformat ke variabel
- `sprintf(text, "nama : %s\numur : %s tahun\n\n", nama, umur)` untuk membuat file "keterangan.txt" yang di dalamnya berisi nama dan umur
```c
sprintf(filePath, "%s/%s", directory, jenis);
sprintf(textPath, "%s/keterangan.txt", filePath);
sprintf(newName, "%s/%s.jpg", filePath, nama);
sprintf(text, "nama : %s\numur : %s tahun\n\n", nama, umur);
```
### Kendala dan Error selama pengerjaan
- Kesalahan pada sintaks `char animals[50][100]` harusnya `char animals[50][100]={0}`
![messageImage_1618657171431](https://user-images.githubusercontent.com/81247727/115494826-998bd300-a290-11eb-8c9c-79c1cfaa97c6.jpg)
- Kendala lainnya yaitu masih belum terbiasa dengan daemon, exec, dan fork
 
## Soal 3
Untuk memastikan argumen yang diberikan pengguna benar, maka diletakkan kode berikut di awal program.
```c
int main(int argc, char *argv[]) {
	if (argc != 2 || (strcmp(argv[1], "-z") != 0 && strcmp(argv[1], "-x") != 0)) {
		printf("Wrong argument\n");
		exit(EXIT_FAILURE);
	}
  ...
}
```
### Poin (a)
Untuk membuat folder dengan format nama `[YYYY-mm-dd_HH:ii:ss]`, dapat digunakan fungsi `strftime` pada C dan memanfaatkan `fork()` serta `exec()` untuk memanggil command `mkdir`. Agar pembuatan folder berjalan setiap 40 detik, digunakan daemon dan diberi jeda waktu 40 detik dengan `sleep(40)`. `while(1)` bermakna program akan terus berjalan hingga ada interupsi yaitu SIGKILL dari file **Killer.sh**.
```c
while (1) {
		char *pathName = getNameByTime();
		
		printf("%s\n", pathName);

		child_id = fork();

		if (child_id == 0) {
			execl("/bin/mkdir", "/bin/mkdir", pathName, NULL);
		}

		...
		sleep(40);
	}
```
Fungsi `getNameByTime()` akan mengembalikan string `YYYY-mm-dd_HH:ii:ss` sesuai waktu pada sistem dengan memanfaatkan fungsi `strftime` pada C.
```c
char* getNameByTime() {
	time_t now = time(NULL);
	struct tm* timePtr = localtime(&now);

	char *name = malloc(50 * sizeof(char));

	strftime(name, 50, "%Y-%m-%d_%X", timePtr);
	return name;
}
```

### Poin (b)
Menggunakan daemon, 10 gambar di-download ke direktori yang telah dibuat dengan iterasi sebanyak 10 kali. Pertama, string `dir` yang akan menjadi direktori lengkap tempat disimpannya gambar beserta nama gambar tersebut diformat dengan `pathName` (nama folder yang dibuat di poin (a)) dan timestamp `YYYY-mm-dd_HH:ii:ss` yang didapatkan dari fungsi `getNameByTime()`. Selanjutnya gambar tersebut didownload dengan ukuran (n%1000) + 50 pixel, di mana n adalah detik Epoch Unix, dengan memanggil command `wget` menggunakan `exec()`. 
```c
if (child_id == 0) {
			int images = 10;
			while (images--) {
				child_id = fork();

				if (child_id == 0) {
					char dir[160], url[64];

					int ext = 50 + (int)time(NULL) % 1000;

					sprintf(dir, "%s/%s.jpg", pathName, getNameByTime());
					sprintf(url, "https://picsum.photos/%d", ext);

					execl("/bin/wget", "/bin/wget", "-q", "-O", dir, url, NULL);
				}
				sleep(5);
			}
      ...
}
```

### Poin (c)
Setelah direktori telah terisi dengan 10 gambar, program akan membuat file **status.txt** yang berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. File **status.txt** dibuat dengan memanggil daemon.
```c
while (wait(NULL) > 0);
child_id = fork();

if (child_id == 0) {
  char *fileName = malloc(100 * sizeof(char));
  sprintf(fileName, "%s/status.txt", pathName);

  FILE *status = fopen(fileName, "w");
  fputs(caesar("Download Success", 5), status);
  fclose(status);
  ...
```
Fungsi `caesar(char* in, int shift)` akan menghasilkan string `in` yang telah dienkripsi dengan metode Caesar Cipher sesuai `shift` yang diinginkan (pada soal ini shift 5).
```c
char* caesar(char* in, int shift) {
	char* out = malloc(20 * sizeof(char));
	int i = 0;

	while (i < strlen(in)) {
		if (in[i] >= 'a' && in[i] <= 'z') {
			out[i] = 'a' + (in[i] - 'a' + shift) % 26;
		} else if (in[i] >= 'A' && in[i] <= 'Z') {
			out[i] = 'A' + (in[i] - 'A' + shift) % 26;
		} else {
			out[i] = in[i];
		}
		i++;
	}
	return out;
}
```
Setelah file tersebut dibuat, direktori (folder) akan di-zip menggunakan command `zip` yang dipanggil dengan `exec()`, kemudian folder tersebut dihapus menggunakan command `rm` yang dipanggil dengan `exec()`, menyisakan file zip-nya saja.
```c
        ...
        char zipName[100];
				
				sprintf(zipName, "%s.zip", pathName);
				execl("/bin/zip", "/bin/zip", "-r", zipName, pathName, NULL);
			}
      
    while (wait(NULL) != child_id);
    execl("/bin/rm", "/bin/rm", "-r", pathName, NULL);
    }
```

### Poin (d) dan (e)
Program killer yang merupakan sebuah program bash digenerate di awal sebelum proses pembuatan folder dan download gambar berjalan. Pertama command `fopen` dijalankan untuk membuat file **Killer.sh** dan diberikan mode `w` untuk menulis isi file. Program akan terlebih dahulu mencetak header bash script yaitu `#!/bin/bash`. Baris-baris selanjutnya dicetak berdasarkan argumen yang diberikan pada eksekusi program utama. Jika argumen adalah `-z`, file **Killer.sh** akan langsung menghentikan seluruh proses yang bernama `soal3`. Jika argumen adalah `-x`, file **Killer.sh** akan menghentikan proses parent berdasarkan pid-nya, dan membiarkan child process berjalan hingga selesai. Setelah penulisan file selesai, file **Killer.sh** ditutup.
```c
FILE *killer = fopen("Killer.sh", "w");
fprintf(killer, "#!/bin/bash\n");

	if (strcmp(argv[1], "-z") == 0)
		fprintf(killer, "rm $0\nkillall -9 ./soal3\n");
	else if (strcmp(argv[1], "-x") == 0)
		fprintf(killer, "rm $0\nkill %d\n", getpid());

fclose(killer);
```
Menggunakan daemon, hak akses file **Killer.sh** kemudian diubah dengan command `chmod u+x Killer.sh` agar user bisa langsung mengeksekusi **Killer.sh** pada saat yang dikehendaki.
```c
pid_t child_id = fork();
if(child_id < 0) exit(0);
if(child_id == 0)
    execl("/bin/chmod", "/bin/chmod", "u+x", "Killer.sh", NULL);
```

### Error/kendala yang dialami selama pengerjaan
- Gambar yang di-download sempat tidak muncul sebagai file gambar, dan folder tidak terhapus setelah di-zip.
- Beberapa kali syntax error seperti pada gambar berikut.\
![Screenshot_30](https://user-images.githubusercontent.com/70105993/115445823-f19ee700-a248-11eb-8ee7-4bb1bb70554f.png)
- Beberapa kali terkendala pada file **Killer.sh** untuk argumen `-x`, di mana pada saat **Killer.sh** dijalankan, parent process dihentikan tetapi child process terus berjalan dan terus menghasilkan child process baru setelah direktori yang sedang diproses berhasil di-zip dan dihapus.\
![Screenshot_33](https://user-images.githubusercontent.com/70105993/115446199-6b36d500-a249-11eb-84aa-c4800f8dbbaf.png)
