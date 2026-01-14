#include "svg.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// Helper structure to capture written SVG strings
struct STestOutput{
    std::vector<std::string> DLines;
    bool DDestroyed = false;

    std::string JoinOutput(){
        std::string Result;

        for (const auto& line : DLines){
            Result += line;
        }
        return Result;
    }
};

// Callback to capture SVG output
svg_return_t write_callback(svg_user_context_ptr user, const char* text){
    if(!user || !text){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    OutPtr->DLines.push_back(text);
    return SVG_OK;
}

// Cleanup callback (just returns OK for testing)
svg_return_t cleanup_callback(svg_user_context_ptr user){
    if(!user){
        return SVG_ERR_NULL;
    }
    STestOutput* OutPtr = static_cast<STestOutput*>(user);
    if(OutPtr->DDestroyed){
        return SVG_ERR_STATE;
    }
    OutPtr->DDestroyed = true;
    return SVG_OK;
}

// --- TEST FIXTURE ---
class SVGTest : public ::testing::Test{
    protected:
        STestOutput DOutput;
        svg_context_ptr DContext = nullptr;

        void SetUp() override{
            DContext = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
            ASSERT_NE(DContext, nullptr);
        }

        void TearDown() override{
            if(DContext){
                svg_destroy(DContext);
                DContext = nullptr;
            }
        }
};

// --- BASIC CREATION TEST ---
TEST_F(SVGTest, CreateAndDestroy){
    svg_context_ptr context = svg_create(write_callback,
                                         cleanup_callback,
                                         &DOutput,
                                         100,
                                         100);

    // Verifies that val1!=val2 
    // it tests if they are in the different memory location,
    // not if they have the difference values. 
    EXPECT_NE(context, nullptr); // we're expecting that it won't be null, so can be expecting true
    // Verifies that val1==val2 (vals being two C strings)
    // it tests if they are in the same memory location,
    // not if they have the same value. 
    EXPECT_EQ(svg_destroy(context),SVG_OK); // 
}

// --- INVALID INPUT TESTS ---
TEST_F(SVGTest, NullContextFunctions){
    svg_point_t center = {50,50};
    svg_point_t start = {15,55}, end = {80,30};
    svg_size_t rect_size = {35,75};
    // if the context passed into a function is NULL,
    // the function should return a SVG_ERR_NULL
    EXPECT_EQ(svg_destroy(NULL), SVG_ERR_NULL);
    EXPECT_EQ(svg_circle(NULL,&center,45,"fill:none; stroke:green; stroke-width:2"),
        SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(NULL,&start,&rect_size,"stroke:green; stroke-width:2"),
        SVG_ERR_NULL);
    EXPECT_EQ(svg_line(NULL,&start,&end,"stroke:green; stroke-width:2"),
        SVG_ERR_NULL);
    EXPECT_EQ(svg_group_begin(NULL, "stroke:green; stroke-width:2"), SVG_ERR_NULL);
    EXPECT_EQ(svg_group_end(NULL), SVG_ERR_NULL);
}

// --- DRAWING TESTS ---
TEST_F(SVGTest, Circle){
    svg_point_t center = {50,50};
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    EXPECT_EQ(svg_circle(context,&center,45,"fill:none; stroke:green; stroke-width:2"),
        SVG_OK);
    // testing if the style in circle is NULL or not
    EXPECT_EQ(svg_circle(context,&center,45,NULL), SVG_OK);
}

TEST_F(SVGTest, Rectangle){
    svg_point_t start = {15,55}, end = {80,30};
    svg_size_t rect_size = {35,75};
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    // testing if style in circle can be NULL or not
    EXPECT_EQ(svg_rect(context,&start,&rect_size,"stroke:green; stroke-width:2"),
        SVG_OK); 
    EXPECT_EQ(svg_rect(context,&start,&rect_size,NULL), SVG_OK);
    EXPECT_EQ(svg_rect(context,&start,&rect_size,"stroke:green; stroke-width:2"),
        SVG_OK); 
    EXPECT_EQ(svg_rect(context,&start,&rect_size,NULL), SVG_OK); 
}

TEST_F(SVGTest, Line){
    svg_point_t start = {15,55}, middle = {35,75}, end = {80,30};
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    EXPECT_EQ(svg_line(context,&start,&middle,"stroke:green; stroke-width:2"),
        SVG_OK);
    EXPECT_EQ(svg_line(context,&middle,&end,"stroke:green; stroke-width:2"),
        SVG_OK);;
    EXPECT_EQ(svg_line(context,&middle,&end,NULL), SVG_OK);;

}

// --- GROUPING TEST ---
TEST_F(SVGTest, Grouping){
/**/
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 0, 0);
    svg_return_t svg_grouping = svg_group_begin(
                            context, // context
                            "stroke:green; stroke-width:2"); // attributes
    EXPECT_EQ(svg_grouping, SVG_OK); 
}

// --- EDGE CASES ---
TEST_F(SVGTest, ZeroDimensions){
    // For radius, width, height,
    // lets call 0 an SVG_ERR_INVALID_ARG.
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    svg_point_t center = {0,0};
    svg_point_t start = {0,0}, middle = {0,0}, end = {0,0};
    svg_size_t rect_size = {0, 0};
    EXPECT_EQ(svg_circle(context,&center,0,"fill:none; stroke:green; stroke-width:2"),
        SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_create(write_callback, cleanup_callback, &DOutput, 0, 0),
        nullptr);
    EXPECT_EQ(svg_line(context,&start,&middle,"stroke:green; stroke-width:2"),
        SVG_OK); // Let's treat line different, The caps with styling could be visible on it.
    EXPECT_EQ(svg_rect(context,&start,&rect_size,"stroke:green; stroke-width:2"),
        SVG_OK); // by the line logic, treat rect differently
}

TEST_F(SVGTest, NullPointPointer){
    // Null point pointer testing null svg_point_t parameter 
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    svg_point_t start = {15,55}, middle = {35,75};
    svg_size_t rect_size = {35,75};
    EXPECT_EQ(svg_circle(context, NULL, 10,"fill:none; stroke:green; stroke-width:2"),
        SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_rect(context,NULL ,&rect_size,"stroke:green; stroke-width:2"),
        SVG_ERR_INVALID_ARG);
    EXPECT_EQ(svg_line(context,NULL,NULL,"stroke:green; stroke-width:2"),
        SVG_ERR_INVALID_ARG); 
}

TEST_F(SVGTest, CreateEdgeCases){
    //  You may want to check passing in various values that should return a failure (NULL in for svg_create).  
    // The cleanup_fn can be null, but write_fn cannot. 
    // Height/width has to be positive.
    EXPECT_EQ(svg_create(NULL, cleanup_callback, &DOutput, 100, 100), 
        nullptr);
    EXPECT_EQ(svg_create(write_callback, NULL, &DOutput, -100, -100), 
        nullptr);
}

TEST_F(SVGTest, DestroyEdgeCases){
    // if destroyed twice 
    // if a group hasn't been closed; ??????
    svg_context_ptr context = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
    svg_return_t destroyed_context = svg_destroy(context);
    EXPECT_EQ(destroyed_context, SVG_OK);
    EXPECT_EQ(svg_destroy(context), SVG_ERR_NULL);
}

// Callback to capture SVG output
svg_return_t write_error_callback(svg_user_context_ptr user, const char* text){
    int *FailureCount = (int *)user;
    if(*FailureCount){
        (*FailureCount)--;
        return SVG_OK;    
    }
    return SVG_ERR_IO;
}

// --- IO Errors ---
TEST_F(SVGTest, IOErrorTest){
    svg_return_t io_error_test = write_error_callback(NULL, "test");
    EXPECT_EQ(io_error_test, SVG_ERR_IO);
}