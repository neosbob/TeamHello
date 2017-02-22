#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class NginxStrConfigTest:public::testing::Test
{	
protected:
	bool ParseString(const std::string config_string)
	{
		std::stringstream config_stream(config_string);
		return parser2.Parse(&config_stream, &out_config2);
	}
	NginxConfigParser parser2;
	NginxConfig out_config2;
};

TEST_F(NginxStrConfigTest,SimpleConfig2)
{
	EXPECT_TRUE(ParseString("test valid;"));
	EXPECT_EQ(1, out_config2.statements_.size()) << "Config has one statement";
	EXPECT_EQ("test", out_config2.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStrConfigTest,InvalidConfig)
{
	EXPECT_FALSE(ParseString("test invalid"));
}

TEST_F(NginxStrConfigTest,NestedConfig)
{
	EXPECT_TRUE(ParseString("path {root file;}"));
}

TEST_F(NginxStrConfigTest,InvalidNestedConfig)
{
	EXPECT_FALSE(ParseString("path {root file}"));
}

TEST_F(NginxStrConfigTest,CommentConfig)
{
	EXPECT_TRUE(ParseString("path root file; #comment;"));
	EXPECT_EQ(1, out_config2.statements_.size()) << "Config has one statement";
}

TEST_F(NginxStrConfigTest,ConfigStatement)
{
	EXPECT_TRUE(ParseString("listen 80; listen 70;"));
	EXPECT_EQ(2, out_config2.statements_.size()) << "Config has two statements";
}


TEST_F(NginxStrConfigTest, MultStatementNestedConfig)
{
	const char *str=
	"server"
	"{ root /data/www;"
	   "location /images/;"
	"}";
	std::string foo(str);
	EXPECT_TRUE(ParseString(foo));
	EXPECT_EQ("/data/www", out_config2.statements_.at(0)->child_block_->statements_.at(0)->tokens_.at(1));
	EXPECT_EQ("location", out_config2.statements_.at(0)->child_block_->statements_.at(1)->tokens_.at(0));
}


//multiple nested config is valid, but did not parse correctly
//fixed it at config_parser.cc file 
TEST_F(NginxStrConfigTest,ValidMultNestedConfig)
{
	const char *str=
	"server"
	"{ location /"
		"{root /data/www;}"
	  "location /images/"
		"{root /data; }"
	"}";
	std::string foo(str);
	EXPECT_TRUE(ParseString(foo));
}

//unmatched parenthesis are invalid, but detected as valid
//bug: need fix 
TEST_F(NginxStrConfigTest,MissingStartBraces)
{
	EXPECT_FALSE(ParseString("server location / root /data/www;}"));
	
}
//another case of unmatched parenthesis (missing end braces)
TEST_F(NginxStrConfigTest,MissingEndBraces)
{
	EXPECT_FALSE(ParseString("server {location / root /data/www;"));
	
}



