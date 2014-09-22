void process_A(void)
{
   pid_t pid = fork();
   if (pid == 0) {
      /* child process */
      char filename[11] =”./processB";
      char *argv[2];
      argv[0] = filename;
      argv[1] = NULL;
      execvp(filename, argv);
 } else {
      /* parent process */
      int i;
      for (i=0; i<10; i++) {
         printf("process A\n");
         sleep(1);
      }
} 
}
