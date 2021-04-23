# Soal Shift Modul 2 Sisop 2021 (Kelompok F12)
#### Nama anggota kelompok:
- Farhan Arifandi (05111940000061)
- Fitrah Mutiara (05111940000126)
- M. Iqbal Abdi (05111940000151)

## Soal 1
Pada program ini, akan dilakukan process daemon dengan sleep setiap `satu` detik untuk mengecek 2 hal
- Apakah sudah waktunya ulang tahun Stevany pada 9 April Pukul 22:22:00
- Apakah sudah 6 jam sebelum waktu ulang tahun Stevany yaitu pada pukul 16:22:00

<br />

### Poin(a) Inisiasi
Blok kode dibawah akan menginisiasi values untuk variabel yang akan digunakan sebagai penanda waktu ulang tahun.
```c
int stev_bhour = 22;
int stev_bmin = 22;
int stev_bday = 9;
int stev_bmonth = 3;
```
Lalu potongan kode ini akan membuat variabel bertipe `time_t` untuk mengambil waktu yang ada pada sistem. Lalu valuesnya akan diinisasi ke struct bertipe `tm'
```c
 time_t now = time(NULL);
 struct tm *t = localtime(&now);
 ```
 <br />

### Poin(b) Check dan jalankan
Blok kode ini akan mengecek apakah nilai dari `struct tm *t` sudah sesuai dengan variabel penanda ulang tahun. Jika iya, maka akan dibentuk child process `child_id` yang digunakan untuk men-zip folder `Musyik`, `Pyoto`, `Fylm` dengan outputnya berupa zip baru `Lopyu_Stevany.zip` menggunakan fungsi `execv()`.

```c
 if (stev_bhour == t->tm_hour && stev_bmin == t->tm_min && t->tm_sec == 0
        && stev_bday == t->tm_mday && stev_bmonth == t->tm_mon)
    {
        pid_t child_id;
        if ((child_id = fork()) == 0)
        {
            char *zipFile[] = {"zip", "-vmqr", "Lopyu_Stevany.zip", "Musyik", "Pyoto", "Fylm", NULL};
            execv("/bin/zip", zipFile);
        }
        while((wait(&status)) > 0);
    }
```
Blok ini akan mengecek apakah waktu sudah menunjukkan 6 jam sebelum waktu ulang tahun. Pada blok kode ini akan dibentuk child process yaitu `child1`. Lalu `child1` akan membentuk kembali 2 child process berupa `pid1` dan `pid2`, dan `pid1` akan membentuk kembali `pid2` sebagai child nya sehingga pada blok ini akan terbentuk 4 process.
- Process pertama merupakan process yang dilakukan oleh `pid2` yang parent nya adalah `pid1`. Akan dilakukan `execv("/bin/wget", ...)` untuk mendownload dari link yang berisi folder `FILM` dam outputnya berupa file `Film_for_Stevany.zip`.
- Process kedua `pid1`menunggu child process nya yaitu`pid2` untuk selesai. Lalu akan dilakukan `execv("/bin/unzip", ...)` pada file `Film_for_Stevany.zip` untuk mengekstak isi file nya
- Process ketiga merupakan process dari `pid2` yang memiliki parent `child1`. Process ini akan membentuk folder baru dengan nama `Fylm` dengan fungsi `execv("/bin/mkdir", ...)`
- Process keempat merupakan process dari `child1` sendiri. Process ini akan menunggu child dirinya `pid2` untuk selesai, lalu akan memindahkan seluruh isi Folder `FILM` ke folder `Fylm` melalui fungsi `moveFiles("FILM","Fylm")`. Setelah itu folder `FILM` yang sudah kosong akan dihapus dengan fungsi `execvp("rm", ...)`

```c
else if (stev_bhour - 6 == t->tm_hour && stev_bmin == t->tm_min && t->tm_sec == 0
            && stev_bday == t->tm_mday && stev_bmonth == t->tm_mon)
    {

        pid_t child1, child2, child3, child4;

        if ((child1 = fork()) == 0)
        {
            pid_t pid1 = fork();
            pid_t pid2 = fork();
            if (pid1 == 0)
            {
                if (pid2 == 0)
                {
                    char *linkfilm[] = {"wget", "--no-check-certificate", "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
                    "-O", "Film_for_Stevany.zip","-q", NULL};
                    execv("/bin/wget", linkfilm);
                }
                else
                {
                    while((wait(&status)) > 0);
                    char *extfilm[] = {"unzip","-qq","Film_for_Stevany.zip",NULL};
                    execv("/bin/unzip", extfilm);
                }
            }
            else
            {
                if (pid2 == 0)
                {
                    char *makefolder1[] = {"mkdir","Fylm", NULL};
                    execv("/bin/mkdir", makefolder1);
                }
                else
                {
                    while((wait(&status)) > 0);
                    moveFiles("FILM", "Fylm");
                    char *rmFilm[] = {"rm", "-fr", "FILM", NULL};
                    execvp("rm", rmFilm);
                }
            }
        }
```
Untuk process selanjutnya untuk file `Musik_for_Stevany.zip` dan `Foto_for_Stevany.zip` sama dengan process diatas
<br />
<br />

### Poin(c) Iterasi setiap File
Blok kode selanjutnya akan membuka `foldername` melalui pointer `*dir`, lalu akan dicek apakah direktori tersebut kosong atau tidak. Jika tidak akan dilakukan iterasi pada isi folder tersebut.
```c
void moveFiles(char *folderName, char *stevFolderName)
{
    int status;
    struct dirent *dp;
    DIR *dir = opendir(folderName);

    if (dir != NULL)
    {
        while ((dp = readdir(dir)))
        {
           ...
        }

        (void)closedir(dir);
    }
}
```
Pada tiap iterasi, akan dibuat child process baru dengan nama `ChildMove`. Jika pada folder yang dibuka memiliki file/folder dengan awalan `(dot)` maka keluar dari process. Jika tidak maka buat array `path`. Pada `path` ini akan diisi nama direktori saat ini. Kemudian akan diambil nama file pertama pada direktory tersebut dengan fungsi `dp->d_name`. Lalu fungsi `execv("/bin/mv",move)` akan memindahkan setiap file pertama pada direktori ke folder `stevFolderName`.
```c
	...
        while ((dp = readdir(dir)))
        {
            pid_t childMove = fork();
            if (childMove == 0 && (strcmp(dp->d_name, ".") == 0 && strcmp(dp->d_name, "..") == 0))
            {
                exit(EXIT_SUCCESS);
            }
            if (childMove == 0)
            {
                //printf("%s\n", dp->d_name);
                char path[200];
                // Construct new path from our base path
                strcpy(path, folderName);
                strcat(path, "/");
                strcat(path, dp->d_name);
                //move files
                char *move[] = {"mv", path, stevFolderName, NULL};
                execv("/bin/mv", move);
            }
            while (wait(&status) > 0);
        }
	...

```

## Soal 2
- Array `animals[50][100]` dibuat untuk menyimpan nama-nama binatang yang terdapat pada berkas gambar.
- String `directory` dibuat untuk menyimpan alamat direktori tempat dibuatnya folder dan sebagai tujuan untuk unzip.
```c
char animals[50][100]={0};
char directory[] = "/home/user/modul2/petshop";
```
### Poin (a)
Direktori `/home/user/modul2/petshop` yang dideklarasikan di atas dibuat terlebih dahulu dengan memanggil command `mkdir`, kemudian file **pets.zip** di-unzip dan isinya disimpan di direktori tersebut.
- `-p` pada `mkdir` digunakan untuk untuk membatalkan pembuatan folder bila nama folder sudah ada.
- `chmod` digunakan untuk mengatur hak akses atau permission terhadap suatu file/direktori kepada user, dalam hal ini `+w` yaitu hak untuk menulis (melakukan operasi) pada file/folder.
- `(strcmp(direntPtr->d_name, ".") != 0) && (strcmp(direntPtr->d_name, "..") != 0))` untuk menghindari direktori `.` (pointer ke direktori tersebut) dan juga `..` (pointer ke direktori parentnya).
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
Pertama-tama, jenis-jenis binatang yang terdapat pada seluruh gambar didata dan dimasukkan ke array `animals[50][100]` yang sudah dideklarasikan sebelumnya. Jumlah dari jenis-jenis binatang yang unik dihitung dan dimasukkan ke variabel `count`.
```c
int i, count=0;
while(wait(&status) > 0);
dir = opendir(directory);
if(dir != NULL){
	while((direntPtr = readdir(dir))){
		if(direntPtr->d_type == DT_REG){
			char fName[100], temp[100];

			memset(fName, 0, sizeof(fName));
			memset(temp, 0, sizeof(temp));
			strcpy(fName, direntPtr->d_name);

			if(direntPtr->d_name[0] != '.'){
				i = 0;
				while (fName[i] != ';'){
					temp[i] = fName[i];
					if(fName[++i] == ';') break;
				}

				bool check = false;
				for(i=0; i<count && (!check); i++){
					if(!strcmp(animals[i], temp)) check = true;
				}

				if(!check) strcpy(animals[count++], temp);
			}
		}
	}
}
```
Kemudian folder dibuat untuk setiap jenis peliharaan yang ada dalam array `animals[50][100]`.
- Iterasi dilakukan sebanyak `count` kali (total jumlah jenis binatang unik). 
- `mkdir` digunakan untuk membuat direktori dengan nama variabel folderName.
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
### Poin (c) dan (d)
Pertama-tama, Jika tipe direktori yang dibaca `direntPtr` merupakan file biasa (dalam hal ini merupakan file gambar), direktori lengkap file gambar disimpan dalam string `source`, dan nama file gambar disimpan dalam string `fName`.
- `direntPtr->d_name` merupakan nama-nama jenis hewan (nama folder yang akan menerima file copy).
- `fileName[strlen(fileName)-4] = '\0'` digunakan untuk mengabaikan extension-nya.
- Pointer `firstAnimal` mengambil jenis dan nama hewan pertama pada nama gambar hingga delimiter `_`, dan `secondAnimal` mengambil jenis dan nama hewan pertama pada nama gambar mulai dari setelah titik akhir `strtok` sebelumnya hingga menemui delimiter `_` atau akhir string .
```c
while(wait(&status) > 0);
dir = opendir(directory);
if(dir != NULL){
	while((direntPtr = readdir(dir))){
		if(direntPtr->d_type == DT_REG){
			char fName[300], source[500];

			sprintf(source, "%s/%s", directory, direntPtr->d_name);
			strcpy(fName, direntPtr->d_name);

			fName[strlen(fName)-4] = '\0';
			
			char *firstAnimal = strtok(fileName, "_");
			char *secondAnimal = strtok(NULL, "_");
			...
```
- Jika `secondAnimal` tidak `NULL` (ada dua hewan dalam gambar), maka `secondAnimal` lebih dahulu diproses di fungsi `group_images`, kemudian `firstAnimal`.
```c
...
			if(secondAnimal) group_images(source, secondAnimal);
			group_images(source, firstAnimal);
...
```
Pada fungsi `group_images`, program akan memindahkan foto ke folder dengan kategori yang sesuai dan di-rename dengan nama peliharaan.
- Jenis, nama, dan umur hewan pada `fName` diambil menggunakan `strtok` hingga menemui delimiter `;`
- `filePath` menyimpan direktori folder target, dan `newName` menyimpan direktori lengkap file gambar beserta namanya 
- `cp` pada `/bin/cp` melakukan copy file ke folder target dan nama gambar diubah sesuai dengan `newName`. 
```c
void group_images(char *img, char *fName){
	int status;
	char	*jenis = strtok(fName, ";"), 
		*nama = strtok(NULL, ";"),
		*umur = strtok(NULL, ";"),
		filePath[300], textPath[400], newName[400], text[200];
	
	sprintf(filePath, "%s/%s", directory, jenis);
   	...
   	sprintf(newName, "%s/%s.jpg", filePath, nama);
	...
	pid_t child_id = fork();		
	if(child_id < 0)  exit(EXIT_FAILURE);
	if(child_id == 0) execl("/bin/cp", "/bin/cp", img, newName, NULL);
	while(wait(&status) > 0);
	...
}

```
Setelah file gambar disalin ke folder sesuai jenis peliharaannya, file gambar yang berada pada di luar folder jenis peliharaan dihapus dengan memanggil command `rm`. Proses diulang untuk gambar selanjutnya setelah penghapusan selesai.
```c
	...
		child_id = fork();
		if(child_id < 0)  exit(EXIT_FAILURE);
		if(child_id == 0) execl("/bin/rm", "/bin/rm", "-rf", source, NULL);

		while(wait(&status) > 0);
		}
	}
}
```
### Poin (e)
File **keterangan.txt** dibuat dan diisi nama dan umur semua peliharaan dalam folder tersebut pada fungsi `group_images` saat pointer `secondAnimal` atau `firstAnimal` diproses dalam fungsi tersebut.
- Jenis, nama, dan umur hewan pada `fName` diambil menggunakan `strtok` hingga menemui delimiter `;`
- `textPath` menyimpan direktori yang akan diisikan file **keterangan.txt**.
- `text` menyimpan template teks untuk data masing-masing peliharaan.
- File **keterangan.txt** pada direktori kerja saat ini dibuka menggunakan fungsi `fopen` dengan parameter `a` yang berarti jika file belum ada maka file dibuat terlebih dahulu, kemudian program akan menulis (append) teks baru mulai dari akhir teks dalam file tersebut. Untuk setiap peliharaan yang sedang diproses dalam fungsi `group_images`, program akan meng-append template `teks` ke file **keterangan.txt** sesuai data yang dihasilkan dengan `strtok`.
```c
void group_images(char *img, char *fName){
	int status;
	char	*jenis = strtok(fName, ";"), 
		*nama = strtok(NULL, ";"),
		*umur = strtok(NULL, ";"),
		filePath[300], textPath[400], newName[400], text[200];
	
	...
   	sprintf(textPath, "%s/keterangan.txt", filePath);
    	...
    
	sprintf(text, "nama : %s\numur : %s tahun\n\n", nama, umur);
	
	...
	
	FILE *keterangan = fopen(textPath, "a");
	if(keterangan){
		fprintf(keterangan, "%s", text);
        	fclose(keterangan);
	}
}

```

### Kendala dan Error selama pengerjaan
- Kesalahan pada sintaks `char animals[50][100]` harusnya `char animals[50][100]={0}`
![messageImage_1618657171431](https://user-images.githubusercontent.com/81247727/115494826-998bd300-a290-11eb-8c9c-79c1cfaa97c6.jpg)
- Kendala lainnya yaitu masih belum terbiasa dengan daemon, exec, dan fork.
 
## Soal 3
Untuk memastikan argumen yang diberikan pengguna benar, maka diletakkan kode berikut di awal program.
```c
int main(int argc, char *argv[]) {
	if (argc != 2 || (strcmp(argv[1], "-z") != 0 && strcmp(argv[1], "-x") != 0)) {
		printf("Wrong argument\n");
		exit(EXIT_FAILURE);
	}
  	...
```
### Poin (a)
Untuk membuat folder dengan format nama `[YYYY-mm-dd_HH:ii:ss]`, dapat digunakan fungsi `strftime` pada C dan memanfaatkan `fork()` serta `exec()` untuk memanggil command `mkdir`. Agar pembuatan folder berjalan setiap 40 detik, digunakan daemon dan diberi jeda waktu 40 detik dengan `sleep(40)` di akhir program (proses download gambar berjalan di dalam blok `while`). `while(1)` bermakna program akan terus berjalan hingga ada interupsi yaitu SIGKILL dari file **Killer.sh**.
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
Fungsi `getNameByTime()` akan mengembalikan string dengan format timestamp `YYYY-mm-dd_HH:ii:ss` sesuai waktu pada sistem dengan memanfaatkan fungsi `strftime` pada C.
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
10 gambar di-download ke direktori yang telah dibuat dengan iterasi sebanyak 10 kali. Pertama, string `dir` yang akan menjadi direktori lengkap tempat disimpannya gambar beserta nama gambar tersebut diformat dengan `pathName` (nama folder yang dibuat di poin (a)) dan timestamp `YYYY-mm-dd_HH:ii:ss` yang didapatkan dari fungsi `getNameByTime()`. Selanjutnya gambar tersebut didownload dengan ukuran (n%1000) + 50 pixel, di mana n adalah detik Epoch Unix (`time(NULL)`), dengan memanggil command `wget` menggunakan `exec()`. 
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
Fungsi `caesar(char* in, int shift)` akan menghasilkan string `in` yang telah dienkripsi dengan metode Caesar Cipher sesuai `shift` yang diinginkan (pada soal ini shift = 5).
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
Hak akses file **Killer.sh** kemudian diubah dengan command `chmod u+x Killer.sh` agar user bisa langsung mengeksekusi **Killer.sh** pada saat yang dikehendaki.
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
