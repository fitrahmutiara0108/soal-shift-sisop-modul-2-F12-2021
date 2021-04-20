# Soal Shift Modul 2 Sisop 2021 (Kelompok F12)
#### Nama anggota kelompok:
- Farhan Arifandi (05111940000061)
- Fitrah Mutiara (05111940000126)
- M. Iqbal Abdi (05111940000151)

## Soal 1

## Soal 2

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
- Beberapa kali syntax error seperti pada gambar berikut.
![Screenshot_30](https://user-images.githubusercontent.com/70105993/115445823-f19ee700-a248-11eb-8ee7-4bb1bb70554f.png)
- Beberapa kali terkendala pada file **Killer.sh** untuk argumen `-x`, di mana pada saat **Killer.sh** dijalankan, parent process dihentikan tetapi child process terus berjalan dan terus menghasilkan child process baru setelah direktori yang sedang diproses berhasil di-zip dan dihapus.
![Screenshot_33](https://user-images.githubusercontent.com/70105993/115446199-6b36d500-a249-11eb-84aa-c4800f8dbbaf.png)
