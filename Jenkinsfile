pipeline{
    agent any 
    stages{
        stage("Build"){
            steps{
                echo "============ building microsfc"
                sh 'mkdir -p build'
                sh 'g++ -g $(find -type f -iregex \'.*\\.cpp\') -o build/microsfc-test.out -I /usr/include -I /usr/local/include/gtest -I /usr/local/include/gmock -I . -L/usr/local/lib/ -lgtest -lgtest_main -lgmock -lpthread'
            }
        }
        stage("Test") {
            steps {
                echo "============ testing  microsfc"
                sh 'build/microsfc-test.out --gtest_output="xml:build/microsfc_test_report.xml"'
            }
        }
    }
    post{
        always{
            echo "======== Finished"
            xunit (
                thresholds: [ skipped(failureThreshold: '0'), failed(failureThreshold: '0') ],
                tools: [ GoogleTest(pattern: 'build/*_test_report.xml') ]
            )
        }
    }
}