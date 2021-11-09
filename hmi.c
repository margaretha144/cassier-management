#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <dirent.h>
#include <time.h>
#include <uuid/uuid.h>

#define REGEX_MATCH \
    "[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}"

static const char *gen_uuid();
static void load_assets();
static short regex_search(const char *filename);

void
append_log(const char *title, const char *state)
{
    FILE *log_file = fopen("hmi.log", "a+");
    time_t now = time(&now);
    if (now == -1)
        fprintf(stderr, "The time function failed\n");

    struct tm *ptm = gmtime(&now);
    if (ptm == NULL)
        fprintf(stderr, "The gmtime function failed\n");

    fprintf(log_file, "%s: %s, %s", title, state, asctime(ptm));
    fclose(log_file);
}

int
main(int argc, char *argv[])
{
    load_assets();

    return 0;
}

static const char *
gen_uuid()
{
    uuid_t binuuid;
    uuid_generate_random(binuuid);
    char *uuid = malloc(37 * sizeof(char));
    uuid_unparse(binuuid, uuid);

    return uuid;
}

static void
load_assets()
{
    DIR *d;
    struct dirent *dir;
    short exist;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            exist = regex_search(dir->d_name);
            if (exist == 0) break;
        }

        if (exist != 0) {
            FILE *gen_database;
            const char *db_uuid = gen_uuid();
            gen_database = fopen(db_uuid, "w");

            append_log("Create database", db_uuid);

            fclose(gen_database);
        }
    }
}

static short
regex_search(const char *filename)
{
    regex_t regex;
    short reti;
    char msgbuf[100];

    reti = regcomp(&regex, REGEX_MATCH, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex.\n");
        exit(1);
    }

    reti = regexec(&regex, filename, 0, NULL, 0);
    if (!reti) {
        printf("Regex Match with %s\n", filename);
        return 0;
    }
    else if (reti == REG_NOMATCH) {
        return 1;
    }
    else {
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }

    regfree(&regex);
}
