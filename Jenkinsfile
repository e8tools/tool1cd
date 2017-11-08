
pipeline {
    agent none
    
    stages {

        stage('Сборка') {
            agent { label 'windows' }

            steps {
                checkout scm

                script {
                    bat '"C:\\Program Files\\CMake\\bin\\cmake.exe" cmake -G "Visual Studio 14" -DBOOST_ROOT="%BOOST_ROOT%" -DBOOST_LIBRARYDIR="%BOOST_LIBRARYDIR%" -DBOOST_INCLUDEDIR="%BOOST_INCLUDEDIR%" -DZLIB_LIBRARY=D:\home\dmpas\workspace\lib\zlib\build\Release\zlibstatic.lib -DZLIB_INCLUDE_DIR=D:\home\dmpas\workspace\lib\zlib .'.stripIndent()
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
