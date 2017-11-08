
pipeline {
    agent none
    
    stages {

        stage('Сборка') {
            agent { label 'windows' }

            steps {
                checkout scm

                script {
                    bat '"C:\\Program Files\\CMake\\bin\\cmake.exe" .'
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
