
pipeline {
    agent none
    
    stages {

        stage('Сборка') {

            agent { label 'windows' }

            steps {

                cleanWs()
            
                checkout scm

                script {
                    bat 'chcp 65001 > nul'
                    bat '"C:\\Program Files\\CMake\\bin\\cmake.exe" -DBOOST_ROOT="%BOOST_ROOT%" -DBOOST_LIBRARYDIR="%BOOST_LIBRARYDIR%" -DBOOST_INCLUDEDIR="%BOOST_INCLUDEDIR%" -DZLIB_LIBRARY="%ZLIB_LIBRARY%" -DZLIB_INCLUDE_DIR="%ZLIB_INCLUDE_DIR%" .'
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
                    junit 'tests.xml'
                }
                
            }

        }
    }
}
