import org.jenkinsci.plugins.pipeline.modeldefinition.Utils

def createStage(String name, boolean condition, Closure body) {
   try {
      stage(name) {
         if (currentBuild.result ==~ '(?i)^(failure)$') {
            echo "${name} skipped due to error in stage before\n"
            Utils.markStageSkippedForConditional(STAGE_NAME)
            return
         }
         if (!condition) {
            echo "${name} skipped due to user request\n"
            Utils.markStageSkippedForConditional(STAGE_NAME)
            return
         }
         
         body.call()
      }
   } catch (e) {
      echo "\n\n\n${e.toString()}\n\n\n"
      currentBuild.result = 'FAILURE'
   }
}

node {
   properties([
      parameters([
         choice(
             choices:      ['Analysis', 'Release'].join('\n')
            ,name:         'buildType'
            ,description:  'Type of the build')
   ])])

   currentBuild.displayName = "${currentBuild.displayName}.${params.buildType}"
   def buildConfiguration = params.buildType.equalsIgnoreCase('Release') ? 'Release' : 'Coverage'

   checkout scm
   
   try {
      createStage('Build', true) {
         sh """
            mkdir -p build
            cd build
            cmake -DCMAKE_BUILD_TYPE=${buildConfiguration} ../Source
            cmake --build .
         """
      }

      createStage('Test', true) {
         try {
            sh """
               cd bin               
               valgrind --leak-check=full --show-reachable=yes --track-origins=yes --xml=yes --xml-file=gsclib.valgrind.result.xml ./gsclib.test --gtest_output=xml:gsclib.test.result.xml
            """
            
            xunit( thresholds: [ skipped(    failureThreshold: '0')
                                ,failed(     failureThreshold: '0')]
                  ,tools:      [GoogleTest(  pattern: 'bin/gsclib.test.result.xml')])
            
            publishValgrind(
               pattern: 'bin/gsclib.valgrind.result.xml'
            )
         } finally {
            archiveArtifacts artifacts: 'bin/gsclib.*.result.xml', flattenDirectories: true
         }
      }
         
      createStage('Analyse', buildConfiguration.equalsIgnoreCase('Coverage')) {
         try {
            sh """
               cd build
               gcovr -x -r ../ -e \".+(gmock|gtest).*[.](cpp|h)\$\" -e \".+(Test|TestBase|Mock|main)[.](cpp|h)\$\" > gsclib.coverage.result.xml
            """
            
            cobertura(
                coberturaReportFile: 'build/gsclib.coverage.result.xml'
               ,autoUpdateHealth:    false, autoUpdateStability: false
               ,failUnhealthy:       false, failUnstable: false
               ,maxNumberOfBuilds:   0
               ,onlyStable:          false
               ,zoomCoverageChart:   false)
         } finally {
            archiveArtifacts artifacts: 'build/gsclib.*.result.xml', flattenDirectories: true
         }
      }
      
      createStage('Deploy', buildConfiguration.equalsIgnoreCase('Release')) {
         sh """
            cd build
            cpack
            printf "\n\n\n"
            dpkg --contents gsc*.deb
            printf "\n\n\n"
         """
         
         archiveArtifacts artifacts: 'build/gsc*.deb', flattenDirectories: true
      }
   } finally { deleteDir() }
}