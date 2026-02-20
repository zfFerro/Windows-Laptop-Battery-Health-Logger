#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>

long extract_mwh_value(const char* line);

int main(void) {
    //Declarations used for battery health noting text file
    const char *userprofile = getenv("USERPROFILE");
	char path[MAX_PATH];
    char start_path[MAX_PATH+7];
    char htmlpath[MAX_PATH];
    snprintf(path, sizeof(path), "%s\\batt-health-log.txt", userprofile);
    snprintf(start_path, sizeof(start_path), "start %s", path);
    snprintf(htmlpath, sizeof(htmlpath), "%s\\battery-report.html", userprofile);

    //Generating report and opening it
    system("powercfg /batteryreport /output %userprofile%\\battery-report.html");
    printf("battery report is ready\n");
	Sleep(500);
	system("start %userprofile%\\battery-report.html");

    //Extracting capacity values from the report
    FILE *file = fopen(htmlpath, "r");
    if (!file) {
        perror("battery-report.html Not found");
        return 0;
    }

    char line[4096];
    long design_cap = 0;
    long full_cap = 0;
    while (fgets(line, sizeof(line), file)) {
        if (!design_cap && strstr(line, "DESIGN CAPACITY")) {
            if (strstr(line, "mWh")) {
                design_cap = extract_mwh_value(line);
            } else {
                if (fgets(line, sizeof(line), file))
                    design_cap = extract_mwh_value(line);
            }
        }
        if (!full_cap && strstr(line, "FULL CHARGE CAPACITY")) {
            if (strstr(line, "mWh")) {
                full_cap = extract_mwh_value(line);
            } else {
                if (fgets(line, sizeof(line), file))
                    full_cap = extract_mwh_value(line);
            }
        }
        if (design_cap && full_cap) break;
    }
    fclose(file);

    //Date generation for the report
    SYSTEMTIME st;
    GetLocalTime(&st);
    int year  = st.wYear;
    int month = st.wMonth;
    int day   = st.wDay;
    char date_str[11];
    snprintf(date_str, sizeof(date_str), "%02d-%02d-%04d", day, month, year);
    
    //Opens text file if found otherwise creating one then opens it
    FILE *f = fopen(path, "r");
    if (f) {
        fclose(f);
        f = fopen(path, "a");
        fprintf(f, "\n\t\t\t%ld mWh  %s  %.2f%%", full_cap, date_str, (full_cap / (double)design_cap) * 100);
        fclose(f);
		system(start_path);
    } else {
        printf("File not found. Creating it...\n");
        f = fopen(path, "w");
        if (f) {
            fprintf(f, "DESIGN CAPACITY:\t%ld mWh\n\nFULL CHARGE CAPACITY:\n\t\t\t%ld mWh  %s  %.2f%%", design_cap, full_cap, date_str, (full_cap / (double)design_cap) * 100);
        } else {
            perror("Error creating file");
            return 1;
        }
	    fclose(f);
		system(start_path);
        }

	return 0;
}



long extract_mwh_value(const char* line) {
    const char* mwh = strstr(line, "mWh");
    if (!mwh) return 0;

    // Step back over the space between number and "mWh"
    const char* p = mwh - 1;
    while (p >= line && *p == ' ') p--;

    //Now walk back over digits and commas/periods (e.g. "62,290")
    const char* end = p + 1;
    while (p >= line && (isdigit((unsigned char)*p) || *p == ',' || *p == '.')) p--;
    p++;

    //Copy only the digit characters into cleaned buffer
    char cleaned[64] = {0};
    int j = 0;
    while (p < end && j < 63) {
        if (isdigit((unsigned char)*p)) cleaned[j++] = *p;
        p++;
    }
    return atol(cleaned);
}