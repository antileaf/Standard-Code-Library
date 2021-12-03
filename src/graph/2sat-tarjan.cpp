if (!ok)
	printf("IMPOSSIBLE\n");
else {
	printf("POSSIBLE\n");

	for (int i = 1; i <= n; i++)
		printf("%d%c", sccid[i * 2 - 1] > sccid[i * 2], i < n ? ' ' : '\n');
}