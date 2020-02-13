pipeline{
    agent any 
    stages{
        stage("Build"){
            steps{
                echo "============ building microsfc"
                sh 'mkdir -p build'
                sh 'g++ -g $(find -type f -iregex \'.*/src/.*\\.cpp\') $(find -type f -iregex \'.*/test/.*\\.cpp\') -o build/microsfc-test.out -I /usr/include -I /usr/local/include/gtest -I /usr/local/include/gmock -I . -L/usr/local/lib/ -lgtest -lgtest_main -lgmock -lpthread -lgcov -fprofile-arcs -ftest-coverage'
            }
        }
        stage("Test") {
            steps {
                echo "============ testing  microsfc"
                sh 'build/microsfc-test.out --gtest_output="xml:build/microsfc_test_report.xml"'
                sh 'gcovr --gcov-exclude=".*gtest.*"  --gcov-exclude=".*test.*" --gcov-exclude=".*example.*" -x -d > coverage.xml'
                echo "============ cleaning "
                sh 'ls *.gcno | xargs -L1 -I{} mv {} build/'
                sh 'ls *.gcda | xargs -L1 -I{} mv {} build/'
                sh 'ls *.gcov | xargs -L1 -I{} mv {} build/'
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
            step([$class: 'CoberturaPublisher', autoUpdateHealth: false, autoUpdateStability: false, coberturaReportFile: '**/coverage.xml', failUnhealthy: false, failUnstable: false, maxNumberOfBuilds: 0, onlyStable: false, sourceEncoding: 'ASCII', zoomCoverageChart: false])
        }
    }
}