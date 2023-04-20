#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h> // stdin/stdout
#include <iostream> // stdin/stdout
#include <string> // string
#include <fstream> // files read/write
#include <stdint.h> 

// the default path where stored student data
const char DEFAULT_INPUT_FILENAME[] = { "list2.txt" };

// save department as an uint8_t instead of a symbol with 2 charactor.
// did not used in current homework
#define DEPARTMENT_NUM 53
static const char departmentTable[DEPARTMENT_NUM][3] = {
	"AC", "AD", "AE", "AI", "AT", "BA", "BB", "BE",
	"CC", "CD", "CE", "CH", "CI", "CD", "CT", "CX",
	"DE", "DT", "EC", "EE", "EN", "EO", "EP", "ES",
	"ET", "FB", "FE", "FL", "FN", "GD", "GE", "GX",
	"HC", "IB", "IM", "MA", "MB", "ME", "MG", "MI",
	"MS", "PA", "PE", "RD", "SA", "SI", "SG", "TB",
	"TC", "TE", "TM", "TX", "VE"
};

uint8_t getDepartmentIdx(const char * s) {
	// get the department index by given symbol (a string with 2 charactors and '\0').
	for (int i = 0; i < DEPARTMENT_NUM; i++) {
		if (strcmp(departmentTable[i], s) == 0) // if found the index
			return i;
	}
	// did not find the index
	return 255;
}

// define the Student structure
struct Student {
	char* name;
	char personalID[9];
	char studentID[10];
	uint8_t age;
	uint8_t department;
	float scores[3];
};

std::string GetFileContents(const char* filename) {
	// get entire contents of file
	std::ifstream ifs(filename, std::ifstream::in);
	if (!ifs.good()) {
		std::cout << "failed to opening \"" << filename << '\"' << std::endl;
		exit(1);
	}

	std::string fileContents;
	// getting file size
	ifs.seekg(0, std::ios_base::end);
	fileContents.resize(ifs.tellg());
	// reading file content
	ifs.seekg(0, std::ios_base::beg);
	ifs.read(&fileContents[0], fileContents.size());
	// close file
	ifs.close();

	return fileContents;
}

int unpackStudentData(std::string& content, Student** studentsData, unsigned int* studentNum) {
	// unpack and create an array to store Student's data

	// get number of students in the file. read first line and convert datatype from string to interger.
	*studentNum = std::stoi(content.substr(0, content.find_first_of('\n')));

	// allocate memory to store student data
	*studentsData = new Student[*studentNum];

	size_t oneStudentContentBeg = content.find_first_of('\n');
	size_t oneStudentContentEnd = oneStudentContentBeg;
	for (unsigned int i = 0; i < *studentNum; i++) {
		// get one line of entire content
		oneStudentContentBeg = oneStudentContentEnd + 1;
		oneStudentContentEnd = content.find_first_of('\n', oneStudentContentBeg);
		//     the content may not have a empty line
		if (oneStudentContentEnd == std::string::npos)
			oneStudentContentEnd = content.length() - 1;
		std::string StudentContent = content.substr(oneStudentContentBeg, oneStudentContentEnd - oneStudentContentBeg);
		Student& currStudent = (*studentsData)[i];

		// nothing can read at this line
		if (StudentContent.size() == 0) {
			if (oneStudentContentEnd == content.length() - 1) {
				// end of file, not enough data
				std::cout << std::endl << "the mount of student data does not match the number of student write in line 1." << std::endl;
				return 1;
			}
			else {
				// skip this line
				continue;
			}
		}

		// convert data type
		char name[80], personalID[10], studentID[10], department[5];
		int age;
		float scores[3];
		sscanf(StudentContent.c_str()
			, " %s %s %s %d %s %f %f %f"
			, name, personalID, studentID, &age, department, scores + 0, scores + 1, scores + 2);

		// save to [studentsData] array
		currStudent.name = new char[strlen(name) + 1];
		strcpy(currStudent.name, name);
		strcpy(currStudent.personalID, personalID);
		strcpy(currStudent.studentID, studentID);
		currStudent.age = age;
		currStudent.scores[0] = scores[0];
		currStudent.scores[1] = scores[1];
		currStudent.scores[2] = scores[2];
		currStudent.department == getDepartmentIdx(department);
	}
}

void DeleteStudentData(Student** studentsData, unsigned int studentNum){
	for (unsigned int i = 0; i < studentNum; i++)
		delete[](*studentsData)[i].name;
	delete[](*studentsData);
}
void SelectionSort(Student* studentsData, unsigned int* sortedIdx, unsigned int studentNum) {
	// sort indices instead of moveing entire Student structure while sorting process
	// init indices
	for (unsigned int i = 0; i < studentNum; i++)
		sortedIdx[i] = i;

	// create colculate average score
	float* scores = new float[studentNum];
	for (unsigned int i = 0; i < studentNum; i++)
		scores[i] = (studentsData[i].scores[0] + studentsData[i].scores[1] + studentsData[i].scores[2]) / 3.0f;

	for (unsigned int sortPos = 0; sortPos < studentNum; sortPos++) {
		// find the index of minimum value in unsorted part
		float minScore = scores[sortedIdx[sortPos]];
		unsigned int minIdx = sortPos;
		for (unsigned int checkPos = sortPos + 1; checkPos < studentNum; checkPos++) {
			if (minScore > scores[sortedIdx[checkPos]]) {
				minScore = scores[sortedIdx[checkPos]];
				minIdx = checkPos;
			}
		}

		// swap index
		if (minIdx == sortPos) continue;
		sortedIdx[minIdx]  = sortedIdx[minIdx] ^ sortedIdx[sortPos];
		sortedIdx[sortPos] = sortedIdx[minIdx] ^ sortedIdx[sortPos];
		sortedIdx[minIdx]  = sortedIdx[minIdx] ^ sortedIdx[sortPos];
	}

	// delete an array with average score
	delete[]scores;
}

void PrintResult(Student* studentsData, unsigned int* indices, unsigned int studentNum) {
	// print title
	printf("Student Record List Sotred Ascendingly according to Final Score\n");
	printf("%6s %-9s%-9s%5s", "Index", "F_NAME", "StudentID", "AGE");
	printf("%5s%5s%5s%6s", "Mid1", "Mid2", "Term", "Final");
	printf("\n");

	// print all student data
	for (unsigned int i = 0; i < studentNum; i++) {
		Student& currStudent = studentsData[indices[i]];

		printf("%4d   %-9s%-9s%5d", i, currStudent.name, currStudent.studentID, currStudent.age & 0xff);
		float avgScore = (currStudent.scores[0] + currStudent.scores[1] + currStudent.scores[2]) / 3.0f;
		printf("%5.0f%5.0f%5.0f%6.0f", currStudent.scores[0], currStudent.scores[1], currStudent.scores[2], avgScore);
		printf("\n");
	}

	// print height Final Score detial
	Student& lastStudent = studentsData[indices[studentNum-1]];
	float highestScore = (lastStudent.scores[0] + lastStudent.scores[1] + lastStudent.scores[2]) / 3.0f;
	printf("The student with the highest score\n");

	// it may have multiple students with highest score
	for (int i = studentNum-1; i >0; i--) {
		Student& currStudent = studentsData[indices[i]];
		float avgScore = (currStudent.scores[0] + currStudent.scores[1] + currStudent.scores[2]) / 3.0f;

		// break if not have highest score
		if (avgScore != highestScore) break;

		// print detial
		printf("%-10s: %s\n", "F_NAME", currStudent.name);
		printf("%-10s: %s\n", "ID", currStudent.personalID);
		printf("%-10s: %s\n", "StudentID", currStudent.studentID);
		printf("%-10s: %d\n", "AGE", currStudent.age & 0xff);
		printf("%-10s: %.0f,%.0f,%.0f\n", "Scores", currStudent.scores[0], currStudent.scores[1], currStudent.scores[2]);
		printf("%-10s: %.0f\n", "Final", avgScore);
	}

}

void PrintStructureSize() {
	printf("struct Student (%u Bytes)\n", (unsigned int)sizeof(Student));
	printf("   |%4s|-%10s (  Size  )\n", "Addr", "VarName");
	printf("   |%04u|-%10s (%2u Bytes)\n", (unsigned int)offsetof(Student, name), "name", (unsigned int)sizeof(Student::name));
	printf("   |%04u|-%10s (%2u Bytes)\n", (unsigned int)offsetof(Student, personalID), "personalID", (unsigned int)sizeof(Student::personalID));
	printf("   |%04u|-%10s (%2u Bytes)\n", (unsigned int)offsetof(Student, studentID), "studentID", (unsigned int)sizeof(Student::studentID));
	printf("   |%04u|-%10s (%2u Bytes)\n", (unsigned int)offsetof(Student, age), "age", (unsigned int)sizeof(Student::age));
	printf("   |%04u|-%10s (%2u Bytes)\n", (unsigned int)offsetof(Student, department), "department", (unsigned int)sizeof(Student::department));
	printf("   |%04u|-%10s (%2u Bytes)\n", (unsigned int)offsetof(Student, scores), "scores", (unsigned int)sizeof(Student::scores));
}

int main(int argc, char* argv[]) {
	std::string inFileName = std::string(DEFAULT_INPUT_FILENAME);
	if (argc >= 2)
		inFileName.assign(argv[1]);

	// read all content of the files
	std::string allStudentDataContent = GetFileContents(inFileName.c_str());

	// get number of students in the file. read first line and convert datatype from string to interger.
	unsigned int studentNum;
	
	// allocate memory to store student data
	Student* studentsData;

	// unpack and create data
	unpackStudentData(allStudentDataContent, &studentsData, &studentNum);

	// sort data
	unsigned int* indices = new unsigned int[studentNum];
	SelectionSort(studentsData, indices, studentNum);

	// print data with specific format
	PrintResult(studentsData, indices, studentNum);

	// delete data
	DeleteStudentData(&studentsData, studentNum);

	// print the Student struct
	//PrintStructureSize();

	// wait for any key press that have time to check the result.
	//getchar(); // same as system("pause")

	return 0;
}