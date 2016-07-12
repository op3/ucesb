BISON_PATCH_PIPELINE=\
	sed -e "s/YYSIZE_T yysize = yyssp - yyss + 1/YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1)/" \
	    -e "s/yystpcpy (yyres, yystr) - yyres/(YYSIZE_T) (yystpcpy (yyres, yystr) - yyres)/" \
	    -e "s/(sizeof(\(.*\)) \/ sizeof(char \*))/(int) (sizeof(\\1) \/ sizeof(char *))/" \
	    -e "s/int \(.*\)char1;/int \\1char1 = 0;/" \
# intentionally empty
