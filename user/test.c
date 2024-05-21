#include "kernel/types.h"
#include "user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

void assert(int state) {
	if (!state) {
		printf("assertion failed!\n");
		exit(1);
	}
};

void before_test() {
	unlink("/dir");
	unlink("/dir1");
	unlink("/dir2");
	unlink("/dir3");
	unlink("/dir1/dir2");
	unlink("/dir1/dir2/dir3");
	unlink("file1");
	unlink("file2");
	unlink("link1");
	unlink("link3");
	unlink("link4");
	mkdir("/dir");
	mkdir("/dir1");
	mkdir("/dir2");
	mkdir("/dir3");
	mkdir("/dir1/dir2");
	mkdir("/dir1/dir2/dir3");
}

void ok(char* name) {
	printf("test %s: OK\n", name);
};

void smoke_test() {
	unlink("file1");
	unlink("link1");
	
	int fd = open("file1", O_CREATE);
	assert(fd >= 0);
	close(fd);
	
	fd = symlink("file1", "link1");
	assert(fd >= 0);
	close(fd);

	char buf[9] = "some str";
	fd = open("link1", O_RDWR);
	assert(fd >= 0);
	int res = write(fd, buf, 9);
	assert(res > 0);
	close(fd);

	fd = open("file1", O_RDONLY);
	assert(fd >= 0);
	res = read(fd, buf, 9);
	assert(res > 0);
	close(fd);
	assert( strcmp("some str", buf) == 0 );
}

void test0(char* filepath, char* linkpath) {
	unlink(filepath);
	unlink(linkpath);
	
	int fd = open(filepath, O_CREATE);
	assert(fd >= 0);
	close(fd);
	
	fd = symlink(filepath, linkpath);
	assert(fd >= 0);
	close(fd);

	char buf[9] = "some str";
	fd = open(linkpath, O_RDWR);
	assert(fd >= 0);
	assert(write(fd, buf, 9) > 0);
	close(fd);

	fd = open(filepath, O_RDONLY);
	assert(fd >= 0);
	assert(read(fd, buf, 9) > 0);
	close(fd);
	assert(strcmp("some str", buf) == 0);
}


void test1(char* filepath, char* link1path, char*link2path) {
	unlink(filepath);
	unlink(link1path);
	unlink(link2path);
	
	int fd = open(filepath, O_CREATE);
	assert(fd >= 0);
	close(fd);
	
	fd = symlink(filepath, link1path);
	assert(fd >= 0);
	close(fd);

	fd = symlink(link1path, link2path);
	assert(fd >= 0);
	close(fd);

	char buf[9] = "some str";
	fd = open(link2path, O_RDWR);
	assert(fd >= 0);
	assert(write(fd, buf, 9) > 0);
	close(fd);

	fd = open(filepath, O_RDONLY);
	assert(fd >= 0);
	assert(read(fd, buf, 9) > 0);
	close(fd);
	assert(strcmp("some str", buf) == 0);
}

void test2(char* filepath, char* linkpath, char* rel_filepath) {
	unlink(filepath);
	unlink(linkpath);
	
	int fd = open(filepath, O_CREATE);
	assert(fd >= 0);
	close(fd);
	
	fd = symlink(rel_filepath, linkpath);
	assert(fd >= 0);
	close(fd);

	char buf[9] = "some str";
	fd = open(linkpath, O_RDWR);
	assert(fd >= 0);
	assert(write(fd, buf, 9) > 0);
	close(fd);

	fd = open(filepath, O_RDONLY);
	assert(fd >= 0);
	assert(read(fd, buf, 9) > 0);
	close(fd);
	assert(strcmp("some str", buf) == 0);
}

void test3(
	char* filepath, char* link1path, char*link2path,
	char* relative_filepath, char* relative_link1path) {
	
	unlink(filepath);
	unlink(link1path);
	unlink(link2path);
	
	int fd = open(filepath, O_CREATE);
	assert(fd >= 0);
	close(fd);
	
	fd = symlink(relative_filepath, link1path);
	assert(fd >= 0);
	close(fd);

	fd = symlink(relative_link1path, link2path);
	assert(fd >= 0);
	close(fd);

	char buf[9] = "some str";
	fd = open(link2path, O_RDWR);
	assert(fd >= 0);
	assert(write(fd, buf, 9) > 0);
	close(fd);

	fd = open(filepath, O_RDONLY);
	assert(fd >= 0);
	assert(read(fd, buf, 9) > 0);
	close(fd);
	assert(strcmp("some str", buf) == 0);
}

void double_self_link(char* link1path, char*link2path) {
	
	unlink(link1path);
	unlink(link2path);
	
	int fd = symlink(link2path, link1path);
	assert(fd >= 0);
	close(fd);

	fd = symlink(link1path, link2path);
	assert(fd >= 0);
	close(fd);

	fd = open(link1path, O_RDWR);
	assert(fd < 0);
}

void self_link(char* linkpath) {
	unlink(linkpath);
	
	int fd = symlink(linkpath, linkpath);
	assert(fd >= 0);
	close(fd);

	fd = open(linkpath, O_RDWR);
	assert(fd < 0);
}

void test4(char* filepath, char* linkpath) {
	unlink(filepath);
	unlink(linkpath);
	
	int fd = symlink(filepath, linkpath);
	assert(fd >= 0);
	close(fd);

	fd = open(linkpath, O_RDWR);
	assert(fd < 0);
}

void test5(
	char* filepath, char* relative_filepath, 
	char* filenoexists, char* relative_filenoexists, char* linkpath) {
	
	unlink(filepath);
	unlink(filenoexists);
	unlink(linkpath);

	int fd = open(filepath, O_CREATE);
	close(fd);

	fd = symlink(relative_filenoexists, linkpath);
	assert(fd >= 0);
	close(fd);

	fd = open(linkpath, O_RDONLY);
	assert(fd < 0);
	close(fd);

	unlink(linkpath);
	fd = symlink(relative_filepath, linkpath);
	assert(fd >= 0);
	close(fd);

	fd = open(linkpath, O_RDONLY);
	assert(fd >= 0);
	close(fd);

	unlink(filenoexists);
}

int main() {
	before_test();
	smoke_test();
	ok("smoke test");

	before_test();
	test1("dir1/file1", "dir2/link1", "dir2/link2");
	ok("smoke test 2");

	before_test();
	test0("/dir2/file1", "/dir3/link1");
	ok("absolute links test");

	before_test();
	test0("file2", "dir/../link3");
	ok("the same catalog");

	before_test();
	test0("dir1/dir2/dir3/../dir3/file", "link4");
	ok("2 level bottom");

	before_test();
	test2("file2", "dir1/../dir1/dir2/dir3/link5", "../../../file2");
	ok("2 level up");

	before_test();
	test1("/dir1/dir2/dir3/file", "/dir1/link1", "/dir1/link2");
	ok("double absolute");

	before_test();
	test3("/dir1/dir2/dir3/file", "/dir1/link1", "/dir1/dir2/link2",
		"/dir1/dir2/dir3/file", "../link1");
	ok("absolute + relative");

	before_test();
	test3("/dir1/dir2/dir3/file", "/dir1/link1", "/dir1/dir2/link2",
		"dir2/dir3/file", "../link1");
	ok("double relative");

	before_test();
	self_link("/dir/link0");
	ok("self link");

	before_test();
	double_self_link("/dir1/link1", "/dir/link0");
	ok("double self link");

	before_test();
	test4("/dir1/no_exists", "/dir1/link10");
	ok("abs link no exists");

	before_test();
	test5(
		"/dir1/dir2/dir3/file50", "dir2/dir3/file50", "/dir1/dir2/dir3/file50",
		"dir2/file50", "/dir1/link50");
	ok("relative link no exists");

	before_test();
	test5(
		"/dir1/dir2/file40", "dir2/file40", "/dir1/dir2/file40",
		"dir2/dir3/file40", "/dir1/link40");
	ok("relative link no exists 2 level up");

	before_test();
	test5(
		"/dir1/dir2/dir3/file40", "file40", "/dir1/dir2/dir3/file40",
		"../../file40", "/dir1/dir2/dir3/link40");
	ok("relative link no exists 2 level bottom");

	before_test();
	
	return 0;
}
