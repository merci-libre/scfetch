#include <dirent.h>
#include <pci/pci.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
/* Hey y'all.
 * This project is currently incomplete.
 * I'm going to remake the entire old project in C.
 * However, implementinga solution to gathering system information
 * entirely in C is a lengthy process and is going to
 * require further work and research to be done.
 *
 * For the meanwhile, I'm going to continue working on this project
 * and leave it be for now.
 *
 * Going to upload it to Github for now just to have the project up!
 *
 * -westwardfishdme
 * */
struct directories {
  char homedirectory;
  char promptsdirectory;
  char prompt_path[512];
};
struct numbers {
  int files;
};

// create index to randomly generate prompts.
int findhomedir(struct directories *directory) {
  ;
  // config folder

  const char *homedir = getenv("HOME");

  // check if user has a home directory.
  DIR *homed = opendir(homedir);
  // printf("finding home folder...\n");

  if (homed == NULL) {
    perror("ERROR: Home directory does not exist \n");
    exit(1);
  }
  closedir(homed);

  // debug info
  // printf("home folder found. directory location: %s\n", homedir);
  // printf("copying to struct homedirectory...\n");
  strcpy(directory->prompt_path, homedir);

  // check to see if user has a .config directory.
  return 0;
}

// check if `.config/sfetch/prompts` exists.
int find_prompts_path(struct directories *directory) {
  const char *path_to_directory = "/.config/shellrandomgreeter/prompts";
  strcat(directory->prompt_path, path_to_directory);

  DIR *prompts_exist = opendir(directory->prompt_path);

  if (prompts_exist == NULL) {
    perror("prompts folder does not exist");
    exit(1);
  }
  closedir(prompts_exist);
  // printf("prompts folder located at: %s\n", directory->prompt_path);

  return 0;
}
// count the files in the prompt directory
int count_files(struct directories *directory, struct numbers *integer) {

  int file_count = 0;
  // message
  // printf("counting the files in %s...\n", directory->prompt_path);

  DIR *prompts_directory;
  prompts_directory = opendir(directory->prompt_path);
  struct dirent *entry;
  struct dirent *d_name;

  while ((entry = readdir(prompts_directory)) != NULL) {
    if (entry->d_type == DT_REG) {
      if (entry->d_name[0] != '.') {
        file_count++;
        // printf("%d %s\n", file_count, entry->d_name);
      } else {
        file_count = file_count - 1;
      }
    }
  }
  closedir(prompts_directory);
  // printf("files in %s: %d\n", directory->prompt_path, file_count);
  integer->files = file_count;

  return 0;
}

// take total number of prompts produced in count_files and create an index of
// each file

// create dynamic table
char **dynamic_table(int size) {
  char **filenames = (char **)malloc(size * sizeof(char *));
  for (int i = 0; i < size; i++) {
    filenames[i] = (char *)malloc(256 * sizeof(char));
  }
  return filenames;
}

// free allocated memory for files
void free_mem(char **filenames, int size) {
  for (int i = 0; i < size; i++) {
    free(filenames[i]);
  }
}

char *choose_prompt(char **filenames, int size) {
  if (size == 0) {
    perror("File is not accessible.\n");
    return NULL;
  }
  int random_index = rand() % size;
  return filenames[random_index];
}

char *filetoarray(struct directories *directory, struct numbers *integer) {
  int pf = integer->files;
  // allocate memory to array
  char **filenames = dynamic_table(pf);
  //
  DIR *prompts_directory;
  prompts_directory = opendir(directory->prompt_path);
  int index = 0;
  struct dirent *entry;
  while ((entry = readdir(prompts_directory)) != NULL) {
    if (entry->d_type == DT_REG) {
      if (entry->d_name[0] != '.') {
        strcpy(filenames[index], entry->d_name);
        index++;
      }
    }
  }
  closedir(prompts_directory);
  srand(time(NULL));

  char *chosenfile = choose_prompt(filenames, pf);

  return chosenfile;
}

int getCPUinfo() {
  FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
  if (cpuinfo == NULL) {
    perror("Error opening /proc/cpuinfo, uh-oh.");
    return 1;
  }

  char line[1024];
  while (fgets(line, sizeof(line), cpuinfo)) {
    if (strstr(line, "model name") != NULL) {
      char *modelname = strstr(line, ":");
      if (modelname != NULL) {
        modelname += 2;
        printf("CPU Model: %s\n", modelname);
        break;
      }
    }
  }
  fclose(cpuinfo);
  return 0;
}

/* does not work, keep commented out.
int getGPUinfo() {
  struct pci_access *pciaccess;
  struct pci_dev *pcidev;

  pciaccess = pci_alloc();
  pci_init(pciaccess);
  pci_scan_bus(pciaccess);

  for (pcidev = pciaccess->devices; pcidev; pcidev->next++) {
    pci_fill_info(pcidev, PCI_FILL_IDENT | PCI_FILL_CLASS);

    if (pcidev->device_class == 0x0300) {
      printf("GPU: 0x%x\n", pcidev->device_id);
    }
  }
  pci_cleanup(pciaccess);
  return 0;
}
*/
int main(void) {
  struct directories directory;
  struct numbers integer;
  findhomedir(&directory);
  find_prompts_path(&directory);
  count_files(&directory, &integer);

  /*
   * prints out the file chosen.
   */
  char *chosenfile = filetoarray(&directory, &integer);
  if (chosenfile != NULL) {
    strcat(directory.prompt_path, "/");
    strcat(directory.prompt_path, chosenfile);
    //  printf("\n%s\n\n", directory.prompt_path);
  } else {
    printf("Files not found! Is the directory empty?\n");
  }
  FILE *fPointer;
  fPointer = fopen(directory.prompt_path, "r");
  if (fPointer != NULL) {
    char c;
    while ((c = fgetc(fPointer)) != EOF) {
      putchar(c);
    }
    fclose(fPointer);
  }

  // system info:
  printf("\nSystem Info:\n");

  getCPUinfo();
  // getGPUinfo();
  return 0;
}
