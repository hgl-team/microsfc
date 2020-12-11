pipeline{
    agent any 
    stages{
        stage("Build"){
            steps{
                echo "============ building microsfc"
                sh 'mkdir -p build'
                sh 'rm -rf build/*'
                sh '(cd build && cmake ..)'
                sh '(cd build && make)'
            }
        }
        stage("Test") {
            steps {
                echo "============ testing  microsfc"
                sh '(cd build && ctest --verbose)'
                sh 'gcovr --gcov-exclude=".*gtest.*"  --gcov-exclude=".*test.*" --gcov-exclude=".*example.*" -x -d > coverage.xml'
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