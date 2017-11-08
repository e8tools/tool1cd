
pipeline {
    agent none
    
    stages {

        stage('Сборка') {
            agent { label 'windows' }

            steps {
                checkout scm

                script {
                    bat '"C:\\Program Files\\CMake\\bin\\cmake.exe" -DBOOST_ROOT="D:\\opt\\boost_1_65_1-14.1-32" -DBOOST_LIBRARYDIR="D:\\opt\\boost_1_65_1-14.1-32\\lib32-msvc-14.1" -DBOOST_INCLUDEDIR="D:\\opt\\boost_1_65_1-14.1-32\\" -DZLIB_LIBRARY=D:\\home\\dmpas\\workspace\\lib\\zlib\\build\\Release\\zlibstatic.lib -DZLIB_INCLUDE_DIR=D:\\home\\dmpas\\workspace\\lib\\zlib .'
                    bat '"C:\\Program Files\\CMake\\bin\\cmake.exe" --build .'
                }
            }
        }

        stage('Тестирование кода пакета WIN') {

            agent { label 'windows' }

            steps {
                checkout scm
                
                script {
                    bat 'chcp 65001 > nul && call opm test'
                    junit 'junit-*.xml'
                }
                
            }

        }
    }
}
