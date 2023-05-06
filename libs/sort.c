char ** tokenize(char *source, const char *delim, int *llen) {
	int listlength = 0, list_size = 0;
	char **list = NULL, *token;

	token = strtok(source, delim);
	while (token) {
		if (listlength + 1 >= list_size) {
			if (!(list = realloc(list, (list_size += 8) * sizeof(*list))))
				die("spmenu: unable to realloc %zu bytes\n", list_size * sizeof(*list));
		}
		if (!(list[listlength] = strdup(token)))
			die("spmenu: unable to strdup %zu bytes\n", strlen(token) + 1);
		token = strtok(NULL, delim);
		listlength++;
	}

	*llen = listlength;
	return list;
}

int arrayhas(char **list, int length, char *item) {
	int i;

	for (i = 0; i < length; i++) {
		size_t len1 = strlen(list[i]);
		size_t len2 = strlen(item);
		if (!fstrncmp(list[i], item, len1 > len2 ? len2 : len1))
			return 1;
	}
	return 0;
}
